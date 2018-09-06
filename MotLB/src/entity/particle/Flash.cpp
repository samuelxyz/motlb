/*
 * Flash.cpp
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#include <Entity.h>
#include <particle/Flash.h>
#include <particle/ShieldEffect.h>
#include <projectile/Projectile.h>
#include <Renderer.h>
#include <unit/Unit.h>
#include <algorithm>
#include <array>
#include <vector>

#include "../../Battle.h"

namespace entity
{

  Flash::Flash(Battle* battle, geometry::Vec2 position, double radius, double dr,
      Values::Color centerColor, Values::Color edgeColor, unsigned int lifetime)
  : Particle(battle, Entity::Team::NEUTRAL, position, geometry::Vec2(), lifetime, Values::Depth::FLASHES),
    boxes(),
    centerColor(centerColor), edgeColor(edgeColor), radius(radius), dr(dr)
  {
  }

  Flash::~Flash()
  {
  }

  void Flash::update()
  {
    Particle::update();
    radius += dr;

    findRelevantBoxes();
  }

  void Flash::render(graphics::Renderer& renderer) const
  {
    // figure out what shape the flash should be

#ifdef MOTLB_DEBUG
    const unsigned int numRays = radius;
    constexpr double rStepMax = 6;
#else
    const unsigned int numRays = radius * 3;
    constexpr double rStepMax = 2;
#endif

    if (numRays < 3)
      return; // can't render this

    std::vector<double> rayAngles;
    rayAngles.reserve(numRays);
    for (unsigned int i = 0; i < numRays; ++i)
      rayAngles.push_back( -Values::PI + Values::TWO_PI * i/numRays );

    // define intervals of obstruction
    struct Interval { double min, max; };
    std::vector<Interval> intervals;

    for (const geometry::Box* b : boxes)
    {
      std::array<geometry::Vec2, 4> corners;
      b->absCorners(corners);

      std::array<double, 4> boxAngles;
      for (unsigned int i = 0; i < 4; ++i)
        boxAngles[i] = (corners[i]-position).getAngle();


      double min = std::min(std::min(boxAngles[0], boxAngles[1]), std::min(boxAngles[2], boxAngles[3]));
      double max = std::max(std::max(boxAngles[0], boxAngles[1]), std::max(boxAngles[2], boxAngles[3]));

      // things might mess up because -pi == pi and angles
      // boxes can't surround the flash because that was excluded by findRelevantBoxes()
      // so each interval also can't span more than pi radians so we can tell if there's trouble
      if (max - min > Values::PI)
      {
        // split this thing up into two intervals
        std::vector<double> above, below;
        for (double& ang : boxAngles)
        {
          if (ang >= 0)
            above.push_back(ang);
          else
            below.push_back(ang);
        }
        intervals.push_back({ -Values::PI, *std::max_element(below.begin(), below.end()) });
        intervals.push_back({  *std::min_element(above.begin(), above.end()), Values::PI });
      }
      else
      {
        // everything's fine, no shenanigans needed
        intervals.push_back({ min, max });
      }
    }

    // add a few extra rays to "catch" any edges better
    for (Interval& interval : intervals)
    {
      double offmin = interval.min - 0.01;
      double offmax = interval.min + 0.01;

      // clamp
      if (offmin < -Values::PI)
        offmin += Values::TWO_PI;
      if (offmax > Values::PI)
        offmin -= Values::TWO_PI;

      rayAngles.push_back(interval.min);
      rayAngles.push_back(interval.max);
      rayAngles.push_back(offmin);
      rayAngles.push_back(offmax);
    }

    // ensure rays will be rendered in the right shape
    std::sort(rayAngles.begin(), rayAngles.end());

    // at this point size of rayAngles is the final number of rays.
    const unsigned int listSize = rayAngles.size();
    struct Ray { geometry::Vec2 vec; bool obstructed; };
    std::vector<Ray> rays;
    rays.reserve(listSize);

    // process rays
    for (unsigned int i = 0; i < listSize; ++i)
    {
      geometry::Vec2 point(position);
      geometry::Vec2 rayDirection;
      rayDirection.setPolar(1, rayAngles[i]);

      bool obstructed = false;
      for (Interval& interval : intervals)
      {
        if (interval.min <= rayAngles[i] && rayAngles[i] <= interval.max)
        {
          obstructed = true;
          break;
        }
      }

      if (obstructed)
      {
        // trace ray

        for (double r = 0; r < radius;)
        {
          // for breaking out of nested loop (stop this ray)
          bool terminateRay = false;

          // boxes will block the ray
          for (const geometry::Box* b : boxes)
          {
            if (b && b->containsAbs(point))
            {
              terminateRay = true;
              break;
            }
          }

          if (terminateRay)
            break;

          double rStep = std::min(rStepMax, radius-r);
          point += rStep * rayDirection;
          r += rStep;
        }
        rays.push_back({ point, true });
      }
      else // not obstructed, don't bother tracing the ray. Just skip to the result
      {
        rays.push_back({ point + radius * rayDirection, false });
      }
    }

    // draw the flash

    Values::CenteredPoly cp;

    Values::Color cColor(centerColor);
    cColor.a = centerColor.a * 1.0f - static_cast<float>(age)/lifetime;
    Values::Color eColor(edgeColor);
    eColor.a = edgeColor.a * 1.0f - static_cast<float>(age)/lifetime;

    cp.push_back( Values::makeCV(cColor, position, depth));

    for (unsigned int i = 0; i < listSize; ++i)
    {
      if (rays[i].obstructed && (rays[i].vec - position).getLength() - radius < 0.1) // this was supposed to hit a box! Trash it
        continue;

      cp.push_back( Values::makeCV(
          Values::interpolateColors(cColor, eColor,
              static_cast<float>((rays[i].vec - position).getLength() / radius) ),
          rays[i].vec,
          depth
      ));
    }

    if (cp.size() < 3)
      return; // can't render this

    renderer.addCenteredPoly(cp);
  }

  void Flash::findRelevantBoxes()
  {
    boxes.clear();

    // Units
    for (Unit* u : battle->getUnits())
    {
      if (u->isActive() &&
          (u->getPosition() - position).getLength() <
            u->getBox().getLongestRadius() + radius &&
          !u->getBox().containsAbs(position))
        boxes.push_back(&(u->getBox()));
    }

    // ShieldEffects
    for (Projectile* p : battle->getProjectiles())
    {
      if (p && p->isActive())
      {
        ShieldEffect* se = dynamic_cast<ShieldEffect*>(p);
        if (se && !se->getBox().containsAbs(position))
          boxes.push_back(&(se->getBox()));
      }
    }
  }

} /* namespace entity */

