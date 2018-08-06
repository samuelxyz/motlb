/*
 * Flash.cpp
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#include <Flash.h>
#include "Particle.h"

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
    findRelevantUnits();

    --timer;
    if (timer <= 0)
      active = false;

  }

  void Flash::render(graphics::Renderer& renderer) const
  {
    const unsigned int numPoints = radius * 6;

    std::array<geometry::Vec2, numPoints> points;
    for (geometry::Vec2& v : points)
      v = position;

    std::array<geometry::Vec2, numPoints> directions;
    for (unsigned int i = 0; i < numPoints; ++i)
      directions[i].setPolar(1, Values::TWO_PI * i/numPoints);

    for (double i = 0; i < radius; ++i)
    {
      if (directions[i].isZero())
        continue;

      for (Unit& u : units)
      {
        if (u.getBox().containsAbs(points[i]))
        {
          directions[i].setRect(0, 0);
          continue;
        }
      }

      points[i] += directions[i];
    }

    Values::CenteredPoly cp;

    cp.push_back(
    {
      centerColor,
      static_cast<float>(position.getX()),
      static_cast<float>(position.getY())
    });

    for (unsigned int i = 0; i < numPoints; ++i)
    {
      cp.push_back(
      {
        Values::interpolateColors(centerColor, edgeColor, static_cast<float>(i)/numPoints),
        static_cast<float>(points[i].getX()),
        static_cast<float>(points[i].getY())
      });
    }

    renderer.addCenteredPoly(cp);

  }

  void Flash::findRelevantUnits()
  {
    for (Unit& u : battle->getUnits())
    {
      if (u.isActive() && (u.getPosition() - position).getLength() <
          Unit::MAX_INTERACTION_DISTANCE + radius)
        units.push_back(u);
    }
  }

} /* namespace entity */

