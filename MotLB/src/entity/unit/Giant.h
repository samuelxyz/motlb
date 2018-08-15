/*
 * Giant.h
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#ifndef GIANT_H_
#define GIANT_H_

#include <unit/Unit.h>

namespace entity
{

  class Giant: public Unit
  {
    public:
      Giant(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Giant();

    protected:
      virtual void attack() override;
  };

} /* namespace entity */

#endif /* GIANT_H_ */
