/*
 * Shield.h
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#ifndef SHIELD_H_
#define SHIELD_H_

#include <Unit.h>
#include <Vec2.h>

namespace entity
{

  class Shield: public Unit
  {
    public:
      Shield(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Shield();

      virtual void update() override;
      virtual void render(graphics::Renderer&) const override;

    protected:
      static constexpr double RANDOM_RICOCHET_ANGLE = 0.3;

      void checkBlock();
      void block(Projectile& p);

  };

} /* namespace entity */

#endif /* SHIELD_H_ */
