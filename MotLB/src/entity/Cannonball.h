/*
 * Cannonball.h
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#ifndef CANNONBALL_H_
#define CANNONBALL_H_

#include <Projectile.h>

namespace entity
{

  class Cannonball: public Projectile
  {
    public:
      Cannonball(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity,
          double damage, double knockback);
      virtual ~Cannonball();

      virtual void update() override;
      virtual void render(graphics::Renderer& renderer) const override;

    protected:
      virtual void hit(Unit&) override;
      virtual void explode() override;

  };

} /* namespace entity */

#endif /* CANNONBALL_H_ */
