/*
 * Unit.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#include <particle/Smoke.h>
#include <unit/Unit.h>
#include <cstdio>
#include <cmath>

#include "../Battle.h"
#include "Values.h"

namespace entity
{

  Unit::Unit(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Entity(battle, team, position, velocity, Values::Depth::UNITS),

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
    checkCollisions();
    checkContainment();

    checkAttack();
  }

  void Unit::render(graphics::Renderer& renderer) const
  {
    if (active)
      renderer.addQuad(Values::makeQuad(getTeamColor(team), box, depth));
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

  void Unit::resurrect()
  {
    if (!active)
    {
      active = true;
      heal();
    }
  }

  void Unit::heal()
  {
    if (active && health < baseHealth)
    {
      health = baseHealth;
      makePoof();
    }
  }

  void Unit::move()
  {
    box.position += velocity;
  }

  bool Unit::checkActive()
  {
    if (active && health <= 0)
    {
      active = false;
      health = 0;
      makePoof();
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
//    if (target == nullptr) // coast to a stop
//      accel /= 10;

    geometry::Vec2 dV = idealVelocity - velocity;
    if (dV.getLength() > accel)
      dV.scaleTo(accel);

    velocity += dV;
  }

  void Unit::checkCollisions()
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

    // move out of overlap
    double totalI = inertia + u.inertia;

    box.position -= dx * (inertia / totalI);
    u.box.position += dx * (inertia / totalI);

    // figure out momentum stuff
    geometry::Vec2 dp = (u.inertia * u.velocity) - (inertia * velocity);

    // inelastic-ish
    double maxSpeed = std::max(velocity.getLength(), u.velocity.getLength());
    double dv = 0.5 * dp.getLength() / std::min(inertia, u.inertia);
    if (dv > maxSpeed)
      dp.scaleTo(2 * std::min(inertia, u.inertia));

    // change velocities, momentum is conserved
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
      if (target)
        attack();
      attackCooldown = attackInterval;
    }
  }

  void Unit::checkContainment()
  {
    geometry::Vec2 dx = battle->getBounds().contain(box);
    box.position += dx;

    if (dx.x)
      velocity.x = 0;
    if (dx.y)
      velocity.y = 0;
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

  void Unit::makePoof() const
  {
    const unsigned int numSmokes = box.getWidth() * box.getHeight() / 100;
    constexpr double posLimit = 0.8;
    for (unsigned int i = 0; i < numSmokes; ++i)
    {
      geometry::Vec2 pos = geometry::Box(box, posLimit).randomInteriorPos();

      geometry::Vec2 baseVel(velocity);
      if (baseVel.getLength() > 2)
        baseVel.scaleTo(2);

      geometry::Vec2 randVel;
      randVel.setPolar(Values::random(), Values::random() * Values::TWO_PI);

      double spin = Values::random(-0.1, 0.1);

      double radius = 0.8 * Values::random(std::min(box.getWidth(), box.getHeight()),
          box.getLongestRadius());
      double dr = Values::random() * 0.2;

      unsigned int lifetime = Values::random(20, 50);

      Values::Color startColor = Entity::getTeamColor(team);
      startColor.a = 0.5f;
      Values::Color endColor(startColor);
      endColor.a = 0.0f;

      battle->add(new Smoke(battle, pos, baseVel + randVel, radius, dr,
          spin, lifetime, startColor, endColor, Values::Depth::LOWER_SMOKE));
    }
  }

  geometry::Vec2 Unit::rayTo(const Unit& other) const
  {
    return (other.getPosition() - getPosition());
  }

} /* namespace entity */

