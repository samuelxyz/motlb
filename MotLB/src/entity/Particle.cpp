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
  Particle::Particle(Battle* battle, Team team, Vec2 position,
      Vec2 velocity, double size, double lifetime)
  : Entity(battle, team, position, velocity),
    size(size), lifetime(lifetime), dSize(-size/lifetime)
  {
  }

  Particle::~Particle()
  {
    // TODO Auto-generated destructor stub
  }

  void Particle::update()
  {
    Entity::update();

    size += dSize;
    if (size <= 0)
      battle->remove(*this);
  }
}
