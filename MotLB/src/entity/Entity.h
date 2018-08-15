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
namespace graphics
{
  class Renderer;
}

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

      Entity(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, float depth);
      virtual ~Entity();

      virtual void update();
      virtual void render(graphics::Renderer& renderer) const;

      bool isActive() const { return active; }
      Team getTeam() const { return team; }

      const float getDepth() { return depth; }
      static const Values::Color& getTeamColor(Team team);

    protected:

      Battle* battle;
      Team team;
      geometry::Vec2 position, velocity;
      bool active;
      const float depth;

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
