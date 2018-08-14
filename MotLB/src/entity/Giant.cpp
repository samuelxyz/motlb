/*
 * Giant.cpp
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#include <Giant.h>
#include "../Battle.h"

namespace entity
{

  Giant::Giant(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Unit(battle, team, position, velocity, angle)
  {
    box = geometry::Box(position, angle, -20, 20, -20, 20);

    attackStrength = 40;
    knockback = 60;

    baseHealth = 600;
    health = baseHealth;

    inertia = 100;
    topSpeed = 0.5;
    acceleration = 0.05;
    rotationSpeed = 0.01;
  }

  Giant::~Giant()
  {
  }

  void Giant::attack()
  {
    geometry::Vec2 attackPoint(box.toAbs(geometry::Vec2(30)));
        for (Unit* u : battle->getUnits())
        {
          if (u->getTeam() != team && u->isActive() &&
              rayTo(*u).getLength() < MAX_INTERACTION_DISTANCE &&
              u->getBox().containsAbs(attackPoint))
          {
            geometry::Vec2 impulse(knockback);
            impulse.rotateBy(box.angle);

            u->receiveAttack(attackStrength, impulse);
          }
        }
  }

} /* namespace entity */
