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
      Vec2 position, Vec2 velocity)
  : Entity(battle, team, position, velocity)
  {
    // TODO Auto-generated constructor stub

  }

  Unit::~Unit()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace entity */
