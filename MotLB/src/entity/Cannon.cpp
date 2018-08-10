/*
 * Cannon.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#include <Cannon.h>
#include <Cannonball.h>
#include <Entity.h>
#include <Renderer.h>
#include <vector>

#include "../Battle.h"
#include "../Values.h"

namespace entity
{

  Cannon::Cannon(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double angle)
  : Gunner(battle, team, position, velocity, angle)
  {
    topSpeed = 0.5;
    attackStrength = 50;
    knockback = 50;
    projVel = 6;
    attackInterval = 200;

    standoffDistance = 400;
  }

  Cannon::~Cannon()
  {
  }

  void Cannon::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);
    Values::Color color = { 0.0f, 0.0f, 0.0f, 0.2f };

    Values::CenteredPoly cp;
    cp.push_back(Values::makeCV(color, box.position));

    constexpr double dotRadius = 6;
    constexpr unsigned int numPoints = 18;
    for (unsigned int i = 0; i < numPoints; ++i)
    {
      geometry::Vec2 v;
      v.setPolar(dotRadius, Values::TWO_PI * i/numPoints);
      cp.push_back(Values::makeCV(color, box.position + v));
    }

    renderer.addCenteredPoly(cp);
  }

  void Cannon::attack()
  {
    if (!target)
      return;

    geometry::Vec2 projVec;
    projVec.setPolar(projVel, box.angle);

    geometry::Vec2 projPos(box.position);
    while (box.containsAbs(projPos))
      projPos += projVec;

    Cannonball* c = new Cannonball(battle, Entity::Team::NEUTRAL,
        projPos, projVec, attackStrength, knockback/projVel);
    battle->add(c);
  }

} /* namespace entity */
