/*
 * Launcher.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#include <Entity.h>
#include <Launcher.h>
#include <Missile.h>
#include <Renderer.h>
#include <Vec2.h>

#include "../Battle.h"
#include "../Values.h"

namespace entity
{

  Launcher::Launcher(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double angle)
  : Gunner(battle, team, position, velocity, angle)
  {
    // Gunner stat overrides
    projVel = 3;
    standoffDistance = 400;
    attackInterval = 100;
    attackCooldown = Values::random() * attackInterval;
  }

  Launcher::~Launcher()
  {
  }

  void Launcher::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    // draw emblem
    geometry::Vec2 forward;
    forward.setPolar(1, box.angle);

    geometry::Vec2 right(forward);
    right.rotateBy(-Values::HALF_PI);

    Values::Color color = { 0.0f, 0.0f, 0.0f, 0.2f };

    Values::Quad quad
    {{
      Values::makeCV( color, box.position + 3*forward + 3*right, Values::Depth::EMBLEMS ),
      Values::makeCV( color, box.position + 3*forward - 3*right, Values::Depth::EMBLEMS ),
      Values::makeCV( color, box.position - 7*forward - 3*right, Values::Depth::EMBLEMS ),
      Values::makeCV( color, box.position - 7*forward + 3*right, Values::Depth::EMBLEMS )
    }};
    renderer.addQuad(quad);

    Values::Triangle tri
    {{
      Values::makeCV( color, box.position + 3*forward + 3*right, Values::Depth::EMBLEMS ),
      Values::makeCV( color, box.position + 3*forward - 3*right, Values::Depth::EMBLEMS ),
      Values::makeCV( color, box.position + 8*forward, Values::Depth::EMBLEMS)
    }};
    renderer.addTriangle(tri);

  }

  void Launcher::attack()
  {
    geometry::Vec2 projVec;
    projVec.setPolar(projVel, box.angle);

    geometry::Vec2 projPos(box.position);
    while (box.containsAbs(projPos))
      projPos += projVec;

    Missile* m = new Missile(battle, team,
        projPos, projVec, attackStrength, knockback, target);
    battle->add(m);
  }

} /* namespace entity */
