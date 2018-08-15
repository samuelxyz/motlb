/*
 * Racer.cpp
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#include <Entity.h>
#include <Renderer.h>
#include <unit/Racer.h>
#include <Vec2.h>

#include "../Values.h"

namespace entity
{

  Racer::Racer(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Unit(battle, team, position, velocity, angle)
  {
    topSpeed = 6;
    rotationSpeed = 0.06;

    attackInterval = 1;
    attackCooldown = 1;
    attackStrength = 3;
    knockback = 3;
    inertia = 30;
  }

  Racer::~Racer()
  {
  }

  void Racer::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    // draw emblem
    geometry::Vec2 v;
    v.setPolar(5, box.angle);

    Values::Color color { 0.0f, 0.0f, 0.0f, 0.2f };
    Values::Triangle tri;
    for (unsigned int i = 0; i < 3; ++i)
    {
      tri[i] = Values::makeCV(color, getPosition() + v, Values::Depth::EMBLEMS);
      v.rotateBy(Values::TWO_PI / 3);
    }
    renderer.addTriangle(tri);
  }

} /* namespace entity */
