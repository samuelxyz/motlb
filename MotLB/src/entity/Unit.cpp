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
    box(position, angle, -.10, .10, -.10, .10),
    active(true),
    health(baseHealth),
    attackCooldown(rand() % attackInterval), // TODO: random
    target(nullptr)
  {
  }

  Unit::~Unit()
  {
  }

  geometry::Vec2 Unit::getPosition()
  {
    return box.position;
  }

  geometry::Vec2 Unit::getAngle()
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
    if (!target)
      return;

    double targetAngle =
        (target->getPosition() - getPosition()).getAngle();

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
      if (u.active && &u != this)
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
  }

  void Unit::attack()
  {
  }

  double Unit::idealSpeed()
  {
    return (target)? topSpeed : 0;
  }

  void Unit::receiveAttack(double damage, geometry::Vec2 impulse)
  {
    health -= damage;
    receiveImpulse(impulse);
  }

  void Unit::receiveImpulse(geometry::Vec2 impulse)
  {
    velocity += (1/inertia) * impulse;
  }

} /* namespace entity */

