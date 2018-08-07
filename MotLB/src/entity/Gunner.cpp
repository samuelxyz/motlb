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
    Unit::render(renderer);

    geometry::Vec2 forward;
    forward.setPolar(4, box.angle);

    geometry::Vec2 right;
    right.rotateBy(-Values::HALF_PI);

    std::array<geometry::Vec2, 4> corners
    {
      box.position + forward,
      box.position + right,
      box.position - 2*forward,
      box.position - right
    };

    Values::Quad quad;
    for (unsigned int i = 0; i < corners.size(); ++i)
    {
      quad[i] =
      {
          { 0.0f, 0.0f, 0.0f, 0.5f },
          static_cast<float>(corners[i].getX()),
          static_cast<float>(corners[i].getY())
      };
    }

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
    if (!target)
      return;

    geometry::Vec2 projVec;
    projVec.setPolar(projVel, box.angle);

    geometry::Vec2 projPos(box.position);
    while (box.containsAbs(projPos))
      projPos += projVec;

    Projectile* p = new Projectile(battle, Entity::Team::NEUTRAL,
        projPos, projVec, attackStrength, knockback, true);
    battle->add(p);
  }

} /* namespace entity */
