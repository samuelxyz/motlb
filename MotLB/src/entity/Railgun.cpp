/*
 * Railgun.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: Samuel Tan
 */

#include <Railgun.h>
#include "../Battle.h"
#include "Flash.h"
#include "Smoke.h"

namespace entity
{

  Railgun::Railgun(Battle* b, Team t, geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Gunner(b, t, position, velocity, angle)
  {
    topSpeed = 0.5;
    standoffDistance = 600;
    rotationSpeed = 0.03;

    baseHealth = 30;
    health = baseHealth;

    projVel = 1;
    attackStrength = 100;
    knockback = 50;
    attackInterval = 200;
    attackCooldown = Values::random() * attackInterval;
  }

  Railgun::~Railgun()
  {
  }

  void Railgun::attack()
  {
    geometry::Box hitArea(getPosition(), box.angle, 0, 800, -3, 3);
    for (Unit* u : battle->getUnits())
    {
      if (u->isActive() && !box.collide(hitArea, u->getBox()).isZero() && !(u == this))
      {
        // hit u
        geometry::Vec2 impulse;
        impulse.setPolar(knockback, box.angle);
        u->receiveAttack(attackStrength, impulse);

        // make flash
        geometry::Vec2 dx;
        dx.setPolar(u->getBox().getLongestRadius() + 0.01, -box.angle);

        battle->add(new Flash
        (
            battle, u->getPosition() + dx, Values::random(40, 60), 0,
            Values::Color { 1.0f, 1.0f, 0.6f, 1.0f },
            Values::Color { 1.0f, 0.8f, 0.8f, 0.1f },
            30
        ));
      }
    }

    // beam effect
    geometry::Vec2 dx;
    dx.setPolar(5, box.angle);

    geometry::Vec2 smokePos(getPosition());

    while (battle->getBounds().containsAbs(smokePos))
    {
      smokePos += dx;
      geometry::Vec2 smokeVel;
      smokeVel.setPolar(Values::random(), Values::random() * Values::TWO_PI);

      battle->add(new Smoke(battle, smokePos, smokeVel,
          Values::random(5, 10), 0, 0,
          Values::random(30, 60),
          Values::Color { 0.0f, 0.0f, 0.0f, 0.2f },
          Values::Color { 0.0f, 0.0f, 0.0f, 0.0f }
      ));
    }

    // flash on hitting the wall
    smokePos -= dx;
    battle->add(new Flash
    (
        battle, smokePos, Values::random(40, 60), 0,
        Values::Color { 1.0f, 1.0f, 0.6f, 1.0f },
        Values::Color { 1.0f, 0.8f, 0.8f, 0.1f },
        30
    ));

  }

  void Railgun::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    // double lines

    double boxHalfWidth = box.getWidth() * 0.1;
    double boxHalfHeight = box.getHeight() * 0.3;

    geometry::Vec2 dx;
    dx.setPolar(boxHalfWidth * 2, box.angle + Values::HALF_PI);

    geometry::Box right = geometry::Box(box.position + dx, box.angle + Values::HALF_PI,
        -boxHalfWidth, boxHalfWidth, -boxHalfHeight, boxHalfHeight);
    geometry::Box left  = geometry::Box(box.position - dx, box.angle + Values::HALF_PI,
        -boxHalfWidth, boxHalfWidth, -boxHalfHeight, boxHalfHeight);

    renderer.addQuad(Values::makeQuad(Values::Color { 0.0f, 0.0f, 0.0f, 0.2f }, right));
    renderer.addQuad(Values::makeQuad(Values::Color { 0.0f, 0.0f, 0.0f, 0.2f }, left ));
  }

} /* namespace entity */