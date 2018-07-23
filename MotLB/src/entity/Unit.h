/*
 * Unit.h
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#ifndef UNIT_H_
#define UNIT_H_

#include "Entity.h"
#include "../Box.h"

namespace entity
{

  class Unit: public Entity
  {
    public:

      Unit(Battle*, Team,
          Vec2 position, Vec2 velocity, double angle);
      virtual ~Unit();

      Vec2 getPosition();
      Vec2 getAngle();

    protected:

      Box* box;
  };

} /* namespace entity */

#endif /* UNIT_H_ */
