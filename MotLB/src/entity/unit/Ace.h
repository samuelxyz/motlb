/*
 * Resurrector.h
 *
 *  Created on: Aug 15, 2018
 *      Author: Samuel Tan
 */

#ifndef ACE_H_
#define ACE_H_

#include <unit/Unit.h>
#include <Vec2.h>

class Battle;

namespace entity
{

  class Ace: public Unit
  {
    public:
      Ace(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Ace();

      virtual void render(graphics::Renderer& renderer) const override;
      virtual void receiveAttack(const double, const geometry::Vec2) override;

    protected:
      virtual void attack() override;
  };

} /* namespace entity */

#endif /* ACE_H_ */
