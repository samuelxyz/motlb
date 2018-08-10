/*
 * Missile.h
 *
 *  Created on: Aug 9, 2018
 *      Author: Samuel Tan
 */

#ifndef MISSILE_H_
#define MISSILE_H_

#include <Projectile.h>

namespace entity
{
  class Missile: public Projectile
  {
    public:
      Missile(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity,
          double damage, double knockback, Unit* target);
      virtual ~Missile();

      virtual void update() override;
      virtual void render(graphics::Renderer&) const override;

    protected:
      Unit* target;
      double rotationSpeed;
      int timer;
      virtual void hit(Unit&) override;
  };

} /* namespace entity */

#endif /* MISSILE_H_ */
