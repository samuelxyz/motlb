/*
 * Particle.h
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Entity.h"

namespace entity
{
  class Particle: public entity::Entity
  {
    public:

      Particle(Battle*, Team, geometry::Vec2 position,
          geometry::Vec2 velocity, double size, double lifetime);
      virtual ~Particle();

      virtual void update() override;

    protected:

      virtual void checkContainment() override;

    private:
      double size;
      double lifetime;
      double dSize;

  };
}

#endif /* PARTICLE_H_ */
