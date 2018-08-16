/*
 * Flash.h
 *
 *  Created on: Aug 6, 2018
 *      Author: Samuel Tan
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <particle/Particle.h>
#include <unit/Unit.h>
#include <Vec2.h>
#include <vector>

#include "../Values.h"

class Battle;

namespace entity
{

  class Flash: public Particle
  {
    private:
      std::vector<Unit*> units;
      Values::Color centerColor, edgeColor;
      double radius;
      double dr;

    public:
      Flash(Battle*, geometry::Vec2 position, double radius, double dr,
          Values::Color centerColor, Values::Color edgeColor, unsigned int lifetime);
      virtual ~Flash();

      void update() override;
      void render(graphics::Renderer&) const override;

    private:
      void findRelevantUnits();

  };

} /* namespace entity */

#endif /* FLASH_H_ */