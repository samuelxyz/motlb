/*
 * Resurrector.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: Samuel Tan
 */

#include <Entity.h>
#include <particle/Flash.h>
#include <Renderer.h>
#include <unit/Ace.h>
#include <vector>

#include "../../Battle.h"
#include "../../Values.h"

namespace entity
{

  Ace::Ace(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Unit(battle, team, position, velocity, angle)
  {
    topSpeed = 0.2;
    rotationSpeed = 0.01;
  }

  Ace::~Ace()
  {
  }

  void Ace::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    // draw emblem: centered square
    renderer.addQuad(Values::makeQuad(
        Values::Color { 0.0f, 0.0f, 0.0f, 0.2f }, geometry::Box(box, 0.67), Values::Depth::EMBLEMS));
  }

  void Ace::receiveAttack(const double damage,
      const geometry::Vec2 impulse)
  {
    // resurrect/heal teammates that aren't other resurrectors or self
    for (Unit* u : battle->getUnits())
      if (u->getTeam() == team && !dynamic_cast<Ace*>(u))
      {
        // one of these will do nothing depending on whether u is active
        u->resurrect();
        u->heal();
      }

    Values::Color centerColor = getTeamColor(team);
    Values::Color edgeColor = centerColor;
    edgeColor.a = 0;

    battle->add(new Flash(battle, getPosition(), velocity, 120, 0, centerColor, edgeColor, 60));

    active = false;
  }

  void Ace::attack()
  {
    // doesn't attack
  }

} /* namespace entity */
