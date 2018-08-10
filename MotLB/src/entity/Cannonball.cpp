/*
 * Cannonball.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#include <Cannonball.h>
#include <Entity.h>
#include <Flash.h>
#include <Renderer.h>
#include <Smoke.h>
#include <Unit.h>
#include <Vec2.h>
#include <vector>

#include "../Battle.h"
#include "../Values.h"

namespace entity
{
  class Flash;
} /* namespace entity */

namespace entity
{

  Cannonball::Cannonball(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double damage, double inertia)
  : Projectile(battle, team, position, velocity, damage, inertia, true),
    smokeRadius(50)
  {
  }

  Cannonball::~Cannonball()
  {
  }

  void Cannonball::update()
  {
    move();
    checkContainment();
    if (mode == Mode::FADE_OUT)
      explode();

    checkHit();
  }

  void Cannonball::render(graphics::Renderer& renderer) const
  {
    if (mode == Mode::FADE_OUT)
      return;

    constexpr double radius = 6;
    constexpr unsigned int numPoints = 18u;

    Values::CenteredPoly cp;
    Values::Color color { 0.0f, 0.0f, 0.0f, 0.8f };

    cp.push_back(Values::makeCV(color, position));
    for (unsigned int i = 0; i < numPoints; ++i)
    {
      geometry::Vec2 v;
      v.setPolar(radius, Values::TWO_PI*i/numPoints);
      cp.push_back(Values::makeCV(color, position + v));
    }

    renderer.addCenteredPoly(cp);
  }

  void Cannonball::hit(Unit& u)
  {
    u.receiveAttack(damage, inertia * velocity);
    explode();
  }

  void Cannonball::explode()
  {
    // back up a bit
    position -= velocity;

    // visual effects

    Flash* flash = new Flash(battle, position, 30, 2, Values::Color{ 1.0f, 1.0f, 0.6f, 1.0f },
        Values::Color{ 1.0f, 0.8f, 0.8f, 0.6f }, 40);

    battle->add(flash);

    // make smoke ring
    const unsigned int smokeCount = 50;
    for (unsigned int i = 0; i < smokeCount; ++i)
    {
      double angle = Values::TWO_PI * i/smokeCount;
      double randAngle = Values::random(
          -Values::TWO_PI/smokeCount * 0.3,
          Values::TWO_PI/smokeCount * 0.3
      );

      geometry::Vec2 dx;
      dx.setPolar(Values::random(30, 35), angle + randAngle);
      geometry::Vec2 vel(dx);
      vel.scaleTo(Values::random(1.5, 2.5));

      battle->add(new Smoke(battle, position + dx, vel,
          Values::random(15, 25), -0.5, Values::random(-0.02, 0.02), Values::random(40, 60),
          Values::Color{0.0f, 0.0f, 0.0f, 0.4f},
          Values::Color{0.0f, 0.0f, 0.0f, 0.1f}
      ));
    }

    // hit & damage surrounding units

    constexpr double attackRadius = 60;
    const double impulse = velocity.getLength() * inertia;
    for (Unit* u : battle->getUnits())
    {
      geometry::Vec2 toUnit(u->getPosition() - position);
      const double distance = toUnit.getLength();

      if (u->isActive() && distance < attackRadius)
      {
        geometry::Vec2 knockback(toUnit);
        knockback.scaleTo(impulse * (attackRadius - distance)/attackRadius);
        const double amount = damage * (attackRadius - distance)/attackRadius;
        u->receiveAttack(amount, knockback);
      }
    }

    active = false;
  }

} /* namespace entity */
