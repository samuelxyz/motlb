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

      Particle(Battle*, Team, Vec2 position,
          Vec2 velocity, double size, double lifetime);
      virtual ~Particle();

      virtual void update() override;

    private:
      double size;
      double lifetime;
      double dSize;

  };
}

#endif /* PARTICLE_H_ */
