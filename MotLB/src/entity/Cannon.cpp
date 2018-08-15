/*
 * Cannon.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#include <Cannon.h>
#include <Cannonball.h>
#include <Entity.h>
#include <Renderer.h>
#include <Smoke.h>
#include <vector>

#include "../Battle.h"
#include "../Values.h"

namespace entity
{

  Cannon::Cannon(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double angle)
  : Gunner(battle, team, position, velocity, angle)
  {
    topSpeed = 0.5;
    attackStrength = 50;
    knockback = 50;
    projVel = 6;
    attackInterval = 200;
    attackCooldown = Values::random() * attackInterval;

    standoffDistance = 400;
  }

  Cannon::~Cannon()
  {
  }

  void Cannon::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    // draw emblem
    Values::Color color = { 0.0f, 0.0f, 0.0f, 0.2f };

    Values::CenteredPoly cp;
    cp.push_back(Values::makeCV(color, box.position, Values::Depth::EMBLEMS));

    constexpr double dotRadius = 6;
    constexpr unsigned int numPoints = 18;
    for (unsigned int i = 0; i < numPoints; ++i)
    {
      geometry::Vec2 v;
      v.setPolar(dotRadius, Values::TWO_PI * i/numPoints);
      cp.push_back(Values::makeCV(color, box.position + v, Values::Depth::EMBLEMS));
    }

    renderer.addCenteredPoly(cp);
  }

  void Cannon::attack()
  {
    geometry::Vec2 projVec;
    projVec.setPolar(projVel, box.angle);

    geometry::Vec2 projPos(box.position);
    while (box.containsAbs(projPos))
      projPos += projVec;

    Cannonball* c = new Cannonball(battle, Entity::Team::NEUTRAL,
        projPos, projVec, attackStrength, knockback/projVel);
    battle->add(c);

    // smoke
    geometry::Vec2 smokePos(projPos);
    geometry::Vec2 dxSmoke(projVec);
    dxSmoke.scaleTo(4);
    for (unsigned int i = 0; i < 5; ++i)
    {
      smokePos += dxSmoke;
      geometry::Vec2 smokeVel;
      smokeVel.setPolar(Values::random(), Values::random() * Values::TWO_PI);
      smokeVel += velocity;

      battle->add(new Smoke(battle, smokePos, smokeVel, i+6, 0,
          Values::random(-0.1, 0.1), 30,
          Values::Color { 0.0f, 0.0f, 0.0f, 0.1f },
          Values::Color { 0.0f, 0.0f, 0.0f, 0.0f },
          Values::Depth::UPPER_SMOKE
      ));
    }
  }

} /* namespace entity */
