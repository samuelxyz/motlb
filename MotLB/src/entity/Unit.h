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

      geometry::Vec2 getPosition() const;
      geometry::Vec2 getAngle() const;

      virtual void update() override;
      virtual void render() override;
      void receiveAttack(const double, const geometry::Vec2);
      void receiveImpulse(const geometry::Vec2);

    protected:

      static constexpr double // measurements are per tick
        inertia = 10,
        acceleration = 0.1,
        topSpeed = 1,
        rotationSpeed = 0.1,

        baseHealth = 100,
        attackStrength = 20,
        knockback = 20,

        MAX_INTERACTION_DISTANCE = 50.0;

        ;
      static constexpr int attackInterval = 30;

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

      double idealSpeed() const;
      void doCollision(Unit& u);

      virtual void move() override;
      virtual void checkContainment() override;

      virtual void attack();

      geometry::Vec2 rayTo(const Unit& other) const;
  };

} /* namespace entity */

#endif /* UNIT_H_ */
