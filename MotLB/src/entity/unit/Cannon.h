/*
 * Cannon.h
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#ifndef CANNON_H_
#define CANNON_H_

#include <unit/Gunner.h>
#include <Vec2.h>

namespace entity
{

  class Cannon: public Gunner
  {
    public:
      Cannon(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Cannon();

      virtual void render(graphics::Renderer&) const override;

    protected:
      virtual void attack() override;
  };

} /* namespace entity */

#endif /* CANNON_H_ */
