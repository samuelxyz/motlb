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

    const unsigned int numPoints = radius * 3;
    constexpr double rStep = 2;

    geometry::Vec2* points = new geometry::Vec2[numPoints];
    for (unsigned int i = 0; i < numPoints; ++i)
      points[i] = position;

    geometry::Vec2* directions = new geometry::Vec2[numPoints];
    for (unsigned int i = 0; i < numPoints; ++i)
      directions[i].setPolar(rStep, Values::TWO_PI * i/numPoints);

    for (unsigned int i = 0; i < numPoints; ++i)
    {
      // simulate ray
      for (double r = 0; r < radius; r += rStep)
      {
        // for breaking out of nested (ray) loop
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

        points[i] += directions[i];
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
            Unit::MAX_INTERACTION_DISTANCE + radius &&
          !u->getBox().containsAbs(position))
        units.push_back(u);
    }
  }

} /* namespace entity */

