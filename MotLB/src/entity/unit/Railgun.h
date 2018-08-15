/*
 * Railgun.h
 *
 *  Created on: Aug 13, 2018
 *      Author: Samuel Tan
 */

#ifndef RAILGUN_H_
#define RAILGUN_H_

#include <unit/Gunner.h>

namespace entity
{

  class Railgun: public Gunner
  {
    public:
      Railgun(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Railgun();

      virtual void render(graphics::Renderer&) const override;

    protected:
      virtual void attack() override;
  };

} /* namespace entity */

#endif /* RAILGUN_H_ */
