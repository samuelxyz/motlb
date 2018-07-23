/*
 * Unit.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#include "Unit.h"

namespace entity
{

  Unit::Unit(Battle* battle, Team team,
      Vec2 position, Vec2 velocity, double angle)
  : Entity(battle, team, position, velocity),
    box(new Box(position, -10, 10, -10, 10, angle))
  {
    // TODO Auto-generated constructor stub

  }

  Unit::~Unit()
  {
    delete box;
  }

  Vec2 Unit::getPosition()
  {
    return box->position;
  }

  Vec2 Unit::getAngle()
  {
    return box->angle;
  }

} /* namespace entity */
