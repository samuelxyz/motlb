/*
 * Entity.h
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "../geometry/Vec2.h"
#include "../Values.h"

//#include "../Battle.h"

class Battle;

namespace entity
{
  class Entity
  {
    public:

      enum class Team
      {
        NEUTRAL = 0,
        RED     = 1,
        GREEN   = 2,
        BLUE    = 3,
        YELLOW  = 4,
      };

      Entity(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity);
      virtual ~Entity();

      virtual void update();
      virtual void render();

      static const Values::Color& getTeamColor(Team team);

    protected:

      Battle* battle;
      Team team;
      geometry::Vec2 position, velocity;

      virtual void move();
      virtual void checkContainment() = 0;

    private:

      static constexpr Values::Color teamColors[] =
      {
        {0.5f, 0.5f, 0.5f, 0.5f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.4f, 1.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f}
      };
  };
}

#endif /* ENTITY_H_ */
