/*
 * Missile.cpp
 *
 *  Created on: Aug 9, 2018
 *      Author: Samuel Tan
 */

#include <Missile.h>
#include <Renderer.h>
#include <Unit.h>

namespace entity
{

  Missile::Missile(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double damage, double inertia,
      Unit* target)
  : Projectile(battle, team, position, velocity, damage, inertia, false),
    target(target),
    rotationSpeed(0.05)
  {
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
  }

  void Missile::render(graphics::Renderer& renderer) const
  {
    // flame
    geometry::Vec2 forward(velocity);
    forward.scaleTo(1);
    geometry::Vec2 right(forward);
    right.rotateBy(-Values::HALF_PI);

    Values::Color baseColor { 1.0f, 1.0f, 0.0f, 1.0f };
    Values::Color tailColor { 1.0f, 0.0f, 0.0f, 0.2f };

    Values::Triangle flame
    {{
      Values::makeCV(baseColor, position - 5 * forward + 3 * right),
      Values::makeCV(baseColor, position - 5 * forward - 3 * right),
      Values::makeCV(tailColor, position - (5 + 0.5*renderLength) * forward)
    }};

    renderer.addTriangle(flame);

    // body
    Values::Color bodyColor { 0.0f, 0.0f, 0.0f, 1.0f };

    Values::Quad body
    {{
      Values::makeCV(bodyColor, position + 5 * forward + 3 * right),
      Values::makeCV(bodyColor, position + 5 * forward - 3 * right),
      Values::makeCV(bodyColor, position - 5 * forward - 3 * right),
      Values::makeCV(bodyColor, position - 5 * forward + 3 * right)
    }};

    renderer.addQuad(body);

    Values::Color noseColor(Entity::getTeamColor(team));
    Values::Triangle nosecone
    {{
      Values::makeCV(noseColor, position + 5 * forward + 3 * right),
      Values::makeCV(noseColor, position + 5 * forward - 3 * right),
      Values::makeCV(noseColor, position + 10 * forward)
    }};

    renderer.addTriangle(nosecone);
  }

} /* namespace entity */
