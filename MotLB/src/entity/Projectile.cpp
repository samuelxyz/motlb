/*
 * Projectile.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#include "../Battle.h"
#include "Projectile.h"

namespace entity
{

  Projectile::Projectile(Battle* battle, Team team, Vec2 position,
      Vec2 velocity, double damage, double inertia, double friendlyFire)
  : Entity(battle, team, position, velocity),
    damage(damage), inertia(inertia),
    friendlyFire(friendlyFire)
  {
  }

  Projectile::~Projectile()
  {
    // TODO Auto-generated destructor stub
  }

  void Projectile::update()
  {
    move();
    checkContainment();
    checkHit();
  }

  void Projectile::checkHit()
  {
    for (Unit u : battle->getUnits())
      if (u.active && (friendlyFire || u.team != team) &&
          u.box->containsAbs(position))
        hit(u);
  }

  // only one hit
  void Projectile::hit(Unit& u)
  {
    u.receiveAttack(damage, velocity * inertia);
    battle->remove(*this);
  }

  void Projectile::checkContainment()
  {
    if (!((*battle).getBounds().containsAbs(position)))
      battle->remove(*this);
  }

} /* namespace entity */
