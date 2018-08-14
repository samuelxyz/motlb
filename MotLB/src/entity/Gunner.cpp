/*
 * Gunner.cpp
 *
 *  Created on: Aug 7, 2018
 *      Author: Samuel Tan
 */

#include <Gunner.h>
#include "Renderer.h"
#include "../Battle.h"

namespace entity
{
  // ugh this is gory
  Gunner::Gunner(Battle* b, Team t, geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Unit(b, t, position, velocity, angle),

    // Gunner stats
    projVel(2),
    standoffDistance(200)
  {
    // Unit stat overrides
    attackStrength = 10;
    knockback = 10;
    topSpeed = 0.7;
  }

  Gunner::~Gunner()
  {
  }

  void Gunner::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    geometry::Vec2 forward;
    forward.setPolar(4, box.angle);

    geometry::Vec2 right(forward);
    right.rotateBy(-Values::HALF_PI);

    Values::Color color = { 0.0f, 0.0f, 0.0f, 0.2f };

    Values::Quad quad
    {{
      Values::makeCV( color, box.position + 0.5*forward + forward   ),
      Values::makeCV( color, box.position + 0.5*forward + right     ),
      Values::makeCV( color, box.position + 0.5*forward - 2*forward ),
      Values::makeCV( color, box.position + 0.5*forward - right     )
    }};

    renderer.addQuad(quad);
  }

  double Gunner::idealSpeed() const
  {
    if (!target)
      return 0.0;

    double targetDistance = rayTo(*target).getLength();

    if (targetDistance < standoffDistance)
      return -topSpeed;
    else if (targetDistance < standoffDistance + 10)
      return 0.0;
    else
      return topSpeed;
  }

  void Gunner::attack()
  {
    geometry::Vec2 projVec;
    projVec.setPolar(projVel, box.angle);

    geometry::Vec2 projPos(box.position);
    while (box.containsAbs(projPos))
      projPos += projVec;

    Projectile* p = new Projectile(battle, Entity::Team::NEUTRAL,
        projPos, projVec, attackStrength, knockback/projVel, true);
    battle->add(p);
  }

} /* namespace entity */
