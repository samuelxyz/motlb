/*
 * Racer.h
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#ifndef RACER_H_
#define RACER_H_

#include <Unit.h>

namespace entity
{

  class Racer: public Unit
  {
    public:
      Racer(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Racer();

      virtual void render(graphics::Renderer&) const override;
  };

} /* namespace entity */

#endif /* RACER_H_ */
