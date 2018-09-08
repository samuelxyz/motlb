/*
 * Shield.cpp
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <Entity.h>
#include <particle/ShieldEffect.h>
#include <unit/Shield.h>
#include <vector>

namespace entity
{

  Shield::Shield(Battle* battle, Team team,
      geometry::Vec2 position, geometry::Vec2 velocity, double angle)
  : Unit(battle, team, position, velocity, angle)
  {
    box = geometry::Box(position, angle, -10, 10, -20, 20);

    baseHealth = 200;
    health = baseHealth;

    inertia = 70;
    topSpeed = 0.7;
    acceleration = 0.07;
    rotationSpeed = 0.01;
  }

  Shield::~Shield()
  {
  }

  void Shield::update()
  {
    Unit::update();
    if (!active)
      return;
    checkBlock();
  }

  void Shield::render(graphics::Renderer& renderer) const
  {
    if (!active)
      return;

    Unit::render(renderer);

    // draw emblem: dark front side
    double halfWidth = box.getWidth() / 2;
    double halfHeight = box.getHeight() / 2;

    geometry::Box shape(getPosition(), box.angle,
        halfWidth * 0.4, halfWidth, -halfHeight, halfHeight);

    renderer.addQuad(Values::makeQuad(
        Values::Color { 0.0f, 0.0f, 0.0f, 0.2f }, shape, Values::Depth::EMBLEMS));
  }

  void Shield::checkBlock()
  {
    geometry::Box area(getPosition(), box.angle, 10, 25, -20, 20);

    for (Projectile* p : battle->getProjectiles())
    {
      if (area.containsAbs(p->position))
        block(*p);
    }
  }

  void Shield::block(Projectile& p)
  {
    if (p.blockingBehavior == Projectile::BlockingBehavior::UNBLOCKABLE ||
        !p.active || p.mode != Projectile::Mode::FLYING)
      return;

    geometry::Vec2 facing;
    facing.setPolar(1, box.angle);
    if (facing * p.velocity < 0)
    {
      p.rebound(box.angle, Values::random(-RANDOM_RICOCHET_ANGLE, RANDOM_RICOCHET_ANGLE));
    }

    // make a visual effect
    geometry::Box visualBox(p.position, box.angle, -8, -3, -15, 15);
    Values::Color startColor = getTeamColor(team);
    Values::Color endColor(startColor);
    endColor.a = 0.0f;
    battle->add(new ShieldEffect(battle, velocity, visualBox, startColor, endColor, 30));
  }

} /* namespace entity */
