/*
 * Unit.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: xinyi
 */

#include <cstdio>

#include "Unit.h"
#include "../Battle.h"

namespace entity
{

  Unit::Unit(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Entity(battle, team, position, velocity),
    box(position, angle, -10, 10, -10, 10),
    active(true),
    health(baseHealth),
    attackCooldown(rand() % attackInterval),
    target(nullptr)
  {
  }

  Unit::~Unit()
  {
  }

  geometry::Vec2 Unit::getPosition() const
  {
    return box.position;
  }

  geometry::Vec2 Unit::getAngle() const
  {
    return box.angle;
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

  void Unit::render()
  {
    battle->renderBox(getTeamColor(team), box);
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
    bool targetsExist = false;

    for (Unit& u : battle->getUnits())
    {
      if (u.team != team && u.active)
      {
        targetsExist = true;

        if (target)
        {
          // candidate already exists. Is this one closer?
          if ((u.getPosition() - getPosition()) <
              (target->getPosition() - getPosition()))
          {
            target = &u;

          }
        }
        else // target == nullptr
        {
          target = &u; // this is the first candidate. We'll start here
        }
      }
    }

    if (!targetsExist)
      target = nullptr;
  }

  void Unit::rotate()
  {
    // TODO fix rotation funkiness

    if (!target)
      return;

    double targetAngle =
        rayTo(*target).getAngle();

    double maxAbs = rotationSpeed;

    double rot = // clamp so abs(rot) < rotationSpeed
        std::max(-maxAbs,
            std::min(maxAbs, targetAngle - box.angle));

    box.angle += rot;
  }

  void Unit::accelerate()
  {
    geometry::Vec2 idealVelocity;
    idealVelocity.setPolar(idealSpeed(), box.angle);

    geometry::Vec2 dV = idealVelocity - velocity;
    if (dV.getLength() > acceleration)
      dV.scaleTo(acceleration);

    velocity += dV;
  }

  void Unit::checkCollision()
  {
    for (Unit& u : battle->getUnits())
      if (u.active && &u != this && rayTo(u).getLength() < 50.0)
        doCollision(u);
  }

  void Unit::doCollision(Unit& u)
  {
    if (!geometry::Box::overlaps(box, u.box))
      return;

    geometry::Vec2 dx = geometry::Box::collide(box, u.box);
    if (dx.isZero())
      return;

    double totalI = inertia + u.inertia;

    box.position -= dx * (inertia / totalI);
    u.box.position += dx * (inertia / totalI);

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

