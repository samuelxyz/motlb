/*
 * Unit.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#include <cstdio>
#include <cmath>

#include "Unit.h"
#include "../Battle.h"

namespace entity
{

  Unit::Unit(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Entity(battle, team, position, velocity),

    // const members
    inertia(10),
    acceleration(0.1),
    topSpeed(1),
    rotationSpeed(0.03),

    baseHealth(100),
    attackStrength(20),
    knockback(20),

    attackInterval(30),

    // non-const
    box(position, angle, -10, 10, -10, 10),
    health(baseHealth),
    attackCooldown(Values::random() * attackInterval),
    target(nullptr)
  {
  }

  Unit::~Unit()
  {
  }

  void Unit::update()
  {
    if (!checkActive())
      return;

    updateTarget();

    rotate();
    accelerate();
    move();
    checkCollision();
    checkContainment();

    checkAttack();
  }

  void Unit::render(graphics::Renderer& renderer) const
  {
    if (active)
      renderer.addQuad(Values::makeQuad(getTeamColor(team), box));
  }

  void Unit::move()
  {
    box.position += velocity;
  }

  bool Unit::checkActive()
  {
    if (health <= 0)
    {
      active = false;
      health = 0;
    }
    return active;
  }

  void Unit::updateTarget()
  {
    target = nullptr;

    for (Unit* u : battle->getUnits())
    {
      if (u->team != team && u->active)
      {
        if (target == nullptr)
        {
          target = u; // this is the first candidate. We'll start here
        }
        else
        {
          // candidate already found. Is this one closer?
          if (rayTo(*u) < rayTo(*target))
          {
            target = u;
          }
        }
      }
    }
  }

  void Unit::rotate()
  {
    if (!target)
      return;

    // idk whether things mess up
    // because of how 2pi == 0 when dealing with angles
    // so i did this

    double targetAngle =
        rayTo(*target).getAngle();

    double currentAngle = box.angle;

    double dr = targetAngle - currentAngle;
    dr = fmod(dr, Values::TWO_PI); // modulo % doesn't work on doubles

    if (dr > Values::PI)
      dr -= Values::TWO_PI;
    else if (dr < -Values::PI)
      dr += Values::TWO_PI;

    double maxAbs = rotationSpeed;

    double rot = // clamp so abs(rot) < rotationSpeed
        std::max(-maxAbs,
            std::min(maxAbs, dr));

    box.angle += rot;
  }

  void Unit::accelerate()
  {
    geometry::Vec2 idealVelocity;
    idealVelocity.setPolar(idealSpeed(), box.angle);

    double accel(acceleration);
    if (target == nullptr) // coast to a stop
      accel /= 10;

    geometry::Vec2 dV = idealVelocity - velocity;
    if (dV.getLength() > accel)
      dV.scaleTo(accel);

    velocity += dV;
  }

  void Unit::checkCollision()
  {
    for (Unit* u : battle->getUnits())
      if (u->active && u != this && rayTo(*u).getLength() < MAX_INTERACTION_DISTANCE)
        doCollision(*u);
  }

  void Unit::doCollision(Unit& u)
  {
    geometry::Vec2 dx = geometry::Box::collide(box, u.box);
    if (dx.isZero())
      return;

    double totalI = inertia + u.inertia;

    box.position -= dx * (inertia / totalI);
    u.box.position += dx * (inertia / totalI);

    geometry::Vec2 dp = (u.inertia * u.velocity) - (inertia * velocity);
    receiveImpulse(0.5 * dp);
    u.receiveImpulse(-0.5 * dp);

    checkContainment();
    u.checkContainment();
  }

  void Unit::checkAttack()
  {
    --attackCooldown;

    if (attackCooldown <= 0)
    {
      attack();
      attackCooldown = attackInterval;
    }
  }

  void Unit::checkContainment()
  {
    box.position += battle->getBounds().contain(box);
//    const geometry::Box& bounds = battle->getBounds();
//    box.position += bounds.contain(box);
  }

  void Unit::attack()
  {
    geometry::Vec2 attackPoint(box.toAbs(geometry::Vec2(15)));
    for (Unit* u : battle->getUnits())
    {
      if (u->team != team && u->active &&
          rayTo(*u).getLength() < MAX_INTERACTION_DISTANCE &&
          u->box.containsAbs(attackPoint))
      {
        geometry::Vec2 impulse(knockback);
        impulse.rotateBy(box.angle);

        u->receiveAttack(attackStrength, impulse);
      }
    }
  }

  double Unit::idealSpeed() const
  {
    return (target)? topSpeed : 0;
  }

  void Unit::receiveAttack(const double damage, const geometry::Vec2 impulse)
  {
    health -= damage;
    receiveImpulse(impulse);
  }

  void Unit::receiveImpulse(const geometry::Vec2 impulse)
  {
    velocity += (1/inertia) * impulse;
  }

  geometry::Vec2 Unit::rayTo(const Unit& other) const
  {
    return (other.getPosition() - getPosition());
  }

} /* namespace entity */

