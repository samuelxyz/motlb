/*
 * ShieldEffect.h
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#ifndef SHIELDEFFECT_H_
#define SHIELDEFFECT_H_

#include <particle/Particle.h>

namespace entity
{

  class ShieldEffect: public Particle
  {
    public:
      ShieldEffect(Battle* battle, geometry::Vec2 velocity, geometry::Box& shape, Values::Color startColor, Values::Color endColor,
          unsigned int lifetime);
      virtual ~ShieldEffect();

      virtual void update() override;
      virtual void render(graphics::Renderer& renderer) const override;

      geometry::Vec2 getPosition() const;
      const geometry::Box& getBox() const;

    protected:
      geometry::Box box;
      Values::Color startColor, endColor;
      unsigned int timer;

      virtual void move() override;
  };

} /* namespace entity */

#endif /* SHIELDEFFECT_H_ */
