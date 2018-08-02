/*
 * Particle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include "Particle.h"

#include "..\Battle.h"

namespace entity
{
  Particle::Particle(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double size, double lifetime)
  : Entity(battle, team, position, velocity),
    size(size), lifetime(lifetime), dSize(-size/lifetime)
  {
  }

  Particle::~Particle()
  {
  }

  void Particle::update()
  {
    Entity::update();

    size += dSize;
    if (size <= 0)
      battle->remove(*this);
  }

  void Particle::checkContainment()
  {
    if (!((*battle).getBounds().containsAbs(position)))
      battle->remove(*this);
  }
}
