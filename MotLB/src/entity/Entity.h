/*
 * Entity.h
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "../Vec2.h"
//#include "../Battle.h"

class Battle;

namespace entity
{
  class Entity
  {
    public:

      enum class Team
      {
        NEUTRAL,
        RED,
        BLUE,
        GREEN,
        YELLOW,
      };

      enum class Type
      {
        PARTICLE,
        PROJECTILE,
        UNIT
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
