/*
 * Projectile.h
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

namespace entity
{

  class Unit;

  class Projectile: public Entity
  {
    public:

      Projectile(Battle*, Team, Vec2 position, Vec2 velocity,
          double damage, double knockback, double friendlyFire);
      virtual ~Projectile();

      virtual void update() override;

    protected:

      double damage, inertia;
      bool friendlyFire;

      void checkHit();
      virtual void hit(Unit&);

      virtual void checkContainment() override;

  };

} /* namespace entity */

#endif /* PROJECTILE_H_ */
