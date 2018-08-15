/*
 * Flash.cpp
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Entity.h>
#include <particle/Flash.h>
#include <particle/ShieldEffect.h>
#include <projectile/Projectile.h>
#include <Renderer.h>
#include <algorithm>

#include "../Battle.h"

namespace entity
{

  Flash::Flash(Battle* battle, geometry::Vec2 position, double radius, double dr,
      Values::Color centerColor, Values::Color edgeColor, unsigned int lifetime)
  : Particle(battle, Entity::Team::NEUTRAL, position, geometry::Vec2(), lifetime, Values::Depth::FLASHES),
    units(),
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

    findRelevantUnits();
  }

  void Flash::render(graphics::Renderer& renderer) const
  {
    // figure out what shape the flash should be

#ifdef MOTLB_DEBUG
    const unsigned int numPoints = radius;
    constexpr double rStepMax = 6;
#else
    const unsigned int numPoints = radius * 3;
    constexpr double rStepMax = 2;
#endif

    if (numPoints < 3)
      return;

    geometry::Vec2* points = new geometry::Vec2[numPoints];
    for (unsigned int i = 0; i < numPoints; ++i)
      points[i] = position;

    geometry::Vec2* directions = new geometry::Vec2[numPoints];
    for (unsigned int i = 0; i < numPoints; ++i)
      directions[i].setPolar(1, Values::TWO_PI * i/numPoints);

    for (unsigned int i = 0; i < numPoints; ++i)
    {
      for (double r = 0; r < radius;)
      {
        // for breaking out of nested loop (stop this ray)
        bool terminateRay = false;

        // units will block the ray
        for (Unit* u : units)
        {
          if (u && u->getBox().containsAbs(points[i]))
          {
            terminateRay = true;
            break;
          }
        }

        // ShieldEffects can also block light
        for (Projectile* p : battle->getProjectiles())
        {
          if (p && p->isActive())
          {
            ShieldEffect* se = dynamic_cast<ShieldEffect*>(p);
            if (se && se->getBox().containsAbs(points[i]))
            {
              terminateRay = true;
              break;
            }
          }
        }

        if (terminateRay)
          break;

        double rStep = std::min(rStepMax, radius-r);
        points[i] += rStep * directions[i];
        r += rStep;
      }
    }

    // draw the flash

    Values::CenteredPoly cp;

    Values::Color cColor(centerColor);
    cColor.a = centerColor.a * 1.0f - static_cast<float>(age)/lifetime;
    Values::Color eColor(edgeColor);
    eColor.a = edgeColor.a * 1.0f - static_cast<float>(age)/lifetime;

    cp.push_back( Values::makeCV(cColor, position, depth));

    for (unsigned int i = 0; i < numPoints; ++i)
    {
      cp.push_back( Values::makeCV(
          Values::interpolateColors(cColor, eColor,
              static_cast<float>((points[i] - position).getLength() / radius) ),
          points[i],
          depth
      ));
    }

    renderer.addCenteredPoly(cp);

    delete[] points;
    delete[] directions;
  }

  void Flash::findRelevantUnits()
  {
    units.clear();

    for (Unit* u : battle->getUnits())
    {
      if (u->isActive() &&
          (u->getPosition() - position).getLength() <
            u->getBox().getLongestRadius() + radius &&
          !u->getBox().containsAbs(position))
        units.push_back(u);
    }
  }

} /* namespace entity */

