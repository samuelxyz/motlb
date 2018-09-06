/*
 * Unit.h
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#ifndef UNIT_H_
#define UNIT_H_

#include <projectile/Projectile.h>
#include "../geometry/Box.h"
#include "Entity.h"

namespace entity
{

  class Unit: public Entity
  {
    public:

      Unit(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Unit();

      geometry::Vec2 getPosition() const { return box.position; }
      geometry::Vec2 getVelocity() const { return velocity; }
      geometry::Vec2 getAngle() const { return box.angle; }
      const geometry::Box& getBox() const { return box; }

      virtual void update() override;
      virtual void render(graphics::Renderer&) const override;
      virtual void receiveAttack(const double, const geometry::Vec2);
      void receiveImpulse(const geometry::Vec2);

      void resurrect();
      void heal();

      static constexpr double MAX_INTERACTION_DISTANCE = 50.0;

    protected:

      double
        inertia,
        acceleration,
        topSpeed,
        rotationSpeed,

        baseHealth,
        attackStrength,
        knockback;

      int attackInterval;

      geometry::Box box;
      double health;
      int attackCooldown;
      Unit* target;

      bool checkActive();
      void updateTarget();
      void rotate();
      void accelerate();
      void checkCollisions();
      void checkAttack();

      virtual double idealSpeed() const;
      void doCollision(Unit& u);

      virtual void move() override;
      virtual void checkContainment() override;

      virtual void attack();

      void makePoof() const;

      geometry::Vec2 rayTo(const Unit& other) const;
  };

} /* namespace entity */

#endif /* UNIT_H_ */
