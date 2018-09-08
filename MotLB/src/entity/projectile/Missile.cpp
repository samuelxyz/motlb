/*
 * Missile.cpp
 *
 *  Created on: Aug 9, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <particle/Flash.h>
#include <particle/Smoke.h>
#include <projectile/Missile.h>
#include <Renderer.h>
#include <unit/Unit.h>


namespace entity
{

  Missile::Missile(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double damage, double inertia,
      Unit* target)
  : Projectile(battle, team, position, velocity, damage, inertia, false, Values::Depth::ABOVE_SMOKE),
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
      dx.scaleTo(-13);
      geometry::Vec2 vel;
      vel.setPolar(0.1, Values::random(0, Values::TWO_PI));

      battle->add(new Smoke(battle, position + dx, vel,
          6, -0.15, Values::random(-0.1, 0.1), 24, start, end,
          Values::Depth::UPPER_SMOKE));
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
      Values::makeCV(baseColor, position - 11 * forward + 2.5 * right, depth),
      Values::makeCV(baseColor, position - 11 * forward - 2.5 * right, depth),
      Values::makeCV(tailColor, position - (11 + 0.5*renderLength) * forward, depth)
    }};

    renderer.addTriangle(flame);

    // don't draw the body if it's exploded - duh
    if (mode == Mode::FADE_OUT)
      return;

    Values::Color bodyColor(Entity::getTeamColor(team));

    Values::Quad body
    {{
      Values::makeCV(bodyColor, position + 3 * right, depth),
      Values::makeCV(bodyColor, position - 3 * right, depth),
      Values::makeCV(bodyColor, position - 9 * forward - 3 * right, depth),
      Values::makeCV(bodyColor, position - 9 * forward + 3 * right, depth)
    }};

    renderer.addQuad(body);

    // don't draw the nosecone if it's going to be in a unit - hackish but looks better
    for (Unit* u : battle->getUnits())
      if (u->isActive() && u->getBox().containsAbs(position + velocity))
        return;

    Values::Triangle nosecone
    {{
      Values::makeCV(bodyColor, position + 3 * right,   depth),
      Values::makeCV(bodyColor, position - 3 * right,   depth),
      Values::makeCV(bodyColor, position + 5 * forward, depth)
    }};

    renderer.addTriangle(nosecone);
  }

  void Missile::hit(Unit& u)
  {
    if (u.getVelocity() * velocity < 0)
      explode(u.getVelocity());
    else
      explode();
    u.receiveAttack(damage, velocity * inertia);
  }

  void Missile::explode(geometry::Vec2 vel /*= Vec2()*/)
  {
    mode = Mode::FADE_OUT;

    Flash* flash = new Flash
    (
        battle, position - 2*velocity, vel, 50, 0,
        Values::Color{ 1.0f, 1.0f, 0.6f, 1.0f },
        Values::Color{ 1.0f, 0.8f, 0.8f, 0.1f },
//        Values::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
//        Values::Color{ 0.8f, 0.8f, 1.0f, 0.0f },
        30
    );

    battle->add(flash);
  }

} /* namespace entity */
