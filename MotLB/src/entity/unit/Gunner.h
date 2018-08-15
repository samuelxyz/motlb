/*
 * Gunner.h
 *
 *  Created on: Aug 7, 2018
 *      Author: Samuel Tan
 */

#ifndef GUNNER_H_
#define GUNNER_H_

#include <unit/Unit.h>

namespace entity
{

  class Gunner: public Unit
  {
    public:
      Gunner(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Gunner();

      virtual void render(graphics::Renderer&) const override;

    protected:
      double projVel, standoffDistance;

      virtual double idealSpeed() const override;
      virtual void attack() override;
  };

} /* namespace entity */

#endif /* GUNNER_H_ */
