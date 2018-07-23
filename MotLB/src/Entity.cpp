/*
 * Entity.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include "Entity.h"

#include "Battle.h"
#include "Box.h"

namespace entity
{
  Entity::Entity(Battle* battle, Team team,
      Vec2 position, Vec2 velocity)
  : battle(battle), team(team),
    position(position), velocity(velocity)
  {
  }

  Entity::~Entity()
  {
  }

  void Entity::update()
  {
    move();
    checkContainment();
  }

  void Entity::move()
  {
    position += velocity;
  }

  void Entity::render()
  {
  }

  void Entity::checkContainment()
  {
    if (!((*battle).getBounds().containsAbs(position)))
      (*battle).remove(*this);
  }
}
