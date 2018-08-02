/*
 * Entity.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include "Entity.h"

#include "../Battle.h"
#include "../geometry/Box.h"

namespace entity
{

  // why do i have to define this again ugh
  constexpr Values::Color Entity::teamColors[];

  Entity::Entity(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity)
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

  const Values::Color& Entity::getTeamColor(Team team)
  {
    return teamColors[static_cast<int>(team)];
  }
}

