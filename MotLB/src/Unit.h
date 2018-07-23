/*
 * Unit.h
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#ifndef UNIT_H_
#define UNIT_H_

#include "Entity.h"

namespace entity
{

  class Unit: public Entity
  {
    public:
      Unit(Battle*, Team,
          Vec2 position, Vec2 velocity);
      virtual ~Unit();
  };

} /* namespace entity */

#endif /* UNIT_H_ */
