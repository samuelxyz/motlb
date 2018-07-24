/*
 * Projectile.h
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Entity.h"

namespace entity
{

  class Projectile: public Entity
  {
    public:
      Projectile();
      virtual ~Projectile();
  };

} /* namespace entity */

#endif /* PROJECTILE_H_ */
