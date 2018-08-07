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
      geometry::Vec2 velocity, double radius, unsigned int lifetime)
  : Entity(battle, team, position, velocity),
    radius(radius), lifetime(lifetime), dr(-radius/lifetime)
  {
  }

  Particle::~Particle()
  {
  }

  void Particle::update()
  {
    radius += dr;
    if (radius <= 0)
    {
      active = false;
      return;
    }

    Entity::update();
  }

  void Particle::checkContainment()
  {
    if (!((*battle).getBounds().containsAbs(position)))
      active = false;
  }
}
