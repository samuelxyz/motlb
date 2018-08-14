/*
 * Missile.cpp
 *
 *  Created on: Aug 9, 2018
 *      Author: Samuel Tan
 */

#include <Missile.h>
#include <Renderer.h>
#include <Unit.h>
#include <Flash.h>
#include <Smoke.h>
#include "../Battle.h"

namespace entity
{

  Missile::Missile(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double damage, double inertia,
      Unit* target)
  : Projectile(battle, team, position, velocity, damage, inertia, false),
    target(target),
    rotationSpeed(0.05),
    timer(0)
  {
    blockingBehavior = BlockingBehavior::EXPLODE_ONLY;
  }

  Missile::~Missile()
  {
  }

  void Missile::update()
  {
    // rotate velocity toward target
    if (target && target->isActive())
    {
      double targetAngle =
          (target->getPosition() - position).getAngle();

      double currentAngle = velocity.getAngle();

      double dr = targetAngle - currentAngle;

      if (dr > Values::PI)
        dr -= Values::TWO_PI;
      else if (dr < -Values::PI)
        dr += Values::TWO_PI;

      double maxAbs = rotationSpeed;

      double rot = // clamp so abs(rot) < rotationSpeed
          std::max(-maxAbs,
              std::min(maxAbs, dr));

      velocity.rotateBy(rot);
    }

    Projectile::update();

    ++timer;

    // make smoke
    if (mode == Mode::FLYING)
    {
      Values::Color start {0.0f, 0.0f, 0.0f, 0.1f};
      Values::Color end {0.1f, 0.1f, 0.1f, 0.0f};

      geometry::Vec2 dx(velocity);
      dx.scaleTo(-8);
      geometry::Vec2 vel;
      vel.setPolar(0.1, Values::random(0, Values::TWO_PI));

      battle->add(new Smoke(battle, position + dx, vel,
          6, -0.15, Values::random(-0.1, 0.1), 24, start, end));
    }
  }

  void Missile::render(graphics::Renderer& renderer) const
  {
    // flame
    geometry::Vec2 forward(velocity);
    forward.scaleTo(1);
    geometry::Vec2 right(forward);
    right.rotateBy(-Values::HALF_PI);

    Values::Color baseColor { 1.0f, 1.0f, 0.0f, 1.0f };
    Values::Color tailColor { 1.0f, 0.6f, 0.3f, 0.8f };

    Values::Triangle flame
    {{
      Values::makeCV(baseColor, position - 6 * forward + 2.5 * right),
      Values::makeCV(baseColor, position - 6 * forward - 2.5 * right),
      Values::makeCV(tailColor, position - (6 + 0.5*renderLength) * forward)
    }};

    renderer.addTriangle(flame);

    if (mode == Mode::FADE_OUT)
      return;

    // body
    Values::Color bodyColor(Entity::getTeamColor(team));

    Values::Quad body
    {{
      Values::makeCV(bodyColor, position + 5 * forward + 3 * right),
      Values::makeCV(bodyColor, position + 5 * forward - 3 * right),
      Values::makeCV(bodyColor, position - 4 * forward - 3 * right),
      Values::makeCV(bodyColor, position - 4 * forward + 3 * right)
    }};

    renderer.addQuad(body);

    Values::Triangle nosecone
    {{
      Values::makeCV(bodyColor, position + 5 * forward + 3 * right),
      Values::makeCV(bodyColor, position + 5 * forward - 3 * right),
      Values::makeCV(bodyColor, position + 10 * forward)
    }};

    renderer.addTriangle(nosecone);

    // nozzle can be invisible?  i t ' s   A R T
//    // nozzle
//    Values::Color nozzleColor { 0.4f, 0.4f, 0.4f, 1.0f };
//    Values::Quad nozzle
//    {{
//      Values::makeCV(nozzleColor, position - 3 * forward + 3 * right),
//      Values::makeCV(nozzleColor, position - 3 * forward - 3 * right),
//      Values::makeCV(nozzleColor, position - 6 * forward - 3 * right),
//      Values::makeCV(nozzleColor, position - 6 * forward + 3 * right),
//    }};
//
//    renderer.addQuad(nozzle);
  }

  void Missile::hit(Unit& u)
  {
    u.receiveAttack(damage, velocity * inertia);
    explode();
  }

  void Missile::explode()
  {
    mode = Mode::FADE_OUT;

    Flash* flash = new Flash
    (
        battle, position - 2*velocity, Values::random(30, 50), 0,
        Values::Color{ 1.0f, 1.0f, 0.6f, 1.0f },
        Values::Color{ 1.0f, 0.8f, 0.8f, 0.1f },
//        Values::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
//        Values::Color{ 0.8f, 0.8f, 1.0f, 0.0f },
        30
    );

    battle->add(flash);
  }

} /* namespace entity */
