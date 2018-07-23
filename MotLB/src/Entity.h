/*
 * Entity.h
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Vec2.h"

class Battle;

namespace entity
{
  class Entity
  {
    public:

      enum Team
      {
        NEUTRAL,
        RED,
        BLUE,
        GREEN,
        YELLOW,
      };

      Entity(Battle*, Team,
          Vec2 position, Vec2 velocity);
      virtual ~Entity();

      virtual void update();
      virtual void render();

    protected:

      Battle* battle;
      Team team;
      Vec2 position, velocity;

      virtual void move();
      virtual void checkContainment();
  };
}

#endif /* ENTITY_H_ */
