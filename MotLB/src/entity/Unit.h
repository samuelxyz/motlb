/*
 * Unit.h
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#ifndef UNIT_H_
#define UNIT_H_

#include "../geometry/Box.h"
#include "Entity.h"
#include "Projectile.h"

namespace entity
{

  class Unit: public Entity
  {
      friend class Projectile;

    public:

      Unit(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Unit();

      geometry::Vec2 getPosition();
      geometry::Vec2 getAngle();

      virtual void update() override;
      virtual void render() override;
      void receiveAttack(double, geometry::Vec2);
      void receiveImpulse(geometry::Vec2);

    protected:

      static constexpr double // measurements are per tick
        inertia = 10,
        acceleration = 0.1,
        topSpeed = 1,
        rotationSpeed = 0.1,

        baseHealth = 100,
        attackStrength = 10,
        knockback = 10

        ;
      static constexpr int attackInterval = 20;

      geometry::Box box;
      bool active;
      double health;
      int attackCooldown;
      Unit* target;

      bool checkActive();
      void updateTarget();
      void rotate();
      void accelerate();
      void checkCollision();
      void checkAttack();

      double idealSpeed();
      void doCollision(Unit& u);

      virtual void move() override;
      virtual void checkContainment() override;

      virtual void attack();


  };

} /* namespace entity */

#endif /* UNIT_H_ */
