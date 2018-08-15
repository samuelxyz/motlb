/*
 * Smoke.h
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#ifndef SMOKE_H_
#define SMOKE_H_

#include <Particle.h>

namespace entity
{

  class Smoke: public Particle
  {
    public:
      Smoke(Battle*, geometry::Vec2 position, geometry::Vec2 velocity,
          double radius, double dr, double spin, unsigned int lifetime,
          Values::Color start, Values::Color end, float depth);
      virtual ~Smoke();

      virtual void update() override;
      virtual void render(graphics::Renderer& renderer) const override;

    protected:
      Values::Color start, end;
      std::vector<geometry::Vec2> corners;
      unsigned int age;
      double spin;
      float depth;
  };

} /* namespace entity */

#endif /* SMOKE_H_ */
