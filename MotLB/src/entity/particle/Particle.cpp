/*
 * Particle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include <Battle.h>
#include <particle/Particle.h>


namespace entity
{
  Particle::Particle(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, unsigned int lifetime, float depth)
  : Entity(battle, team, position, velocity, depth),
    lifetime(lifetime), age(0)
  {
  }

  Particle::~Particle()
  {
  }

  void Particle::update()
  {
    ++age;
    if (age > lifetime)
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
