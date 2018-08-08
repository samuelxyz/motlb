/*
 * Flash.cpp
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Entity.h>
#include <Flash.h>
#include <Renderer.h>

#include "../Battle.h"

namespace entity
{

  Flash::Flash(Battle* battle, geometry::Vec2 position, double radius,
      Values::Color centerColor, Values::Color edgeColor, unsigned int lifetime)
  : Particle(battle, Entity::Team::NEUTRAL, position, geometry::Vec2(), radius, lifetime),
    units(), timer(lifetime),
    centerColor(centerColor), edgeColor(edgeColor)
  {
  }

  Flash::~Flash()
  {
  }

  void Flash::update()
  {
    --timer;
    if (timer <= 0)
    {
      active = false;
      return;
    }

    findRelevantUnits();
  }

  void Flash::render(graphics::Renderer& renderer) const
  {
    // figure out what shape the flash should be

#ifdef MOTLB_DEBUG
    const unsigned int numPoints = radius * 3;
    constexpr double rStepMax = 6;
#else
    const unsigned int numPoints = radius * 6;
    constexpr double rStepMax = 4;
#endif

    geometry::Vec2* points = new geometry::Vec2[numPoints];
    for (unsigned int i = 0; i < numPoints; ++i)
      points[i] = position;

    geometry::Vec2* directions = new geometry::Vec2[numPoints];
    for (unsigned int i = 0; i < numPoints; ++i)
      directions[i].setPolar(1, Values::TWO_PI * i/numPoints);

    for (unsigned int i = 0; i < numPoints; ++i)
    {
      // simulate ray
      for (double r = 0; r < radius;)
      {
        // for breaking out of nested loop (stop this ray)
        bool terminateRay = false;
        for (Unit* u : units)
        {
          if (u && u->getBox().containsAbs(points[i]))
          {
            terminateRay = true;
            break;
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
    cColor.a = centerColor.a * timer/lifetime;
    Values::Color eColor(edgeColor);
    eColor.a = edgeColor.a * timer/lifetime;

    cp.push_back(
    {
      cColor,
      static_cast<float>(position.getX()),
      static_cast<float>(position.getY())
    });

    for (unsigned int i = 0; i < numPoints; ++i)
    {
      cp.push_back(
      {
        Values::interpolateColors(cColor, eColor,
            static_cast<float>((points[i] - position).getLength() / radius) ),
        static_cast<float>(points[i].getX()),
        static_cast<float>(points[i].getY())
      });
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

