/*
 * Projectile.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Entity.h>
#include <particle/Flash.h>
#include <projectile/Projectile.h>
#include <Renderer.h>
#include <unit/Unit.h>
#include <Vec2.h>
#include <array>
#include <vector>

#include "../Battle.h"
#include "../Values.h"

namespace entity
{

  Projectile::Projectile(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double damage, double inertia, bool friendlyFire,
      float depth /*= Values::Depth::PROJECTILES*/)
  : Entity(battle, team, position, velocity, depth),
    damage(damage), inertia(inertia),
    friendlyFire(friendlyFire),

    mode(Mode::FLYING),
    blockingBehavior(BlockingBehavior::EXPLODE_AND_REBOUND),
    renderLength(0)
  {
  }

  Projectile::~Projectile()
  {
  }

  void Projectile::update()
  {
    // should be no need for %active checks -
    // inactive projectiles are automatically removed by the %battle

    doFading();
    move();
    checkContainment();
    checkHit();
  }

  void Projectile::render(graphics::Renderer& renderer) const
  {
    geometry::Vec2 forward(velocity);
    forward.scaleTo(1);
    geometry::Vec2 right(forward);
    right.rotateBy(-Values::HALF_PI);

    Values::Color bodyColor { 1.0f, 1.0f, 1.0f, 1.0f };
    Values::Color tailColor { 1.0f, 1.0f, 1.0f, 0.2f };

    std::array<geometry::Vec2, 4> corners
    {
      position + 5   * forward,
      position + 3   * right,
      position - renderLength * forward,
      position - 3   * right
    };

    Values::Quad quad;

    for (unsigned int i = 0; i < corners.size(); i++)
    {
      if (i == 2u)
        quad[i] = Values::makeCV(tailColor, corners[i], depth);
      else
        quad[i] = Values::makeCV(bodyColor, corners[i], depth);
    }

    renderer.addQuad(quad);

  }

  void Projectile::doFading()
  {
    switch (mode)
    {
      case Mode::FLYING:
        if (renderLength < 50)
          ++renderLength;
        break;
      case Mode::FADE_OUT:
        renderLength -= 10;
        if (renderLength <= 0)
          active = false;
        break;
    }
  }

  void Projectile::checkHit()
  {
    if (mode == Mode::FADE_OUT)
      return;

    for (Unit* u : battle->getUnits())
      if (u && u->isActive() &&
          (friendlyFire || u->getTeam() != team) &&
          u->getBox().containsAbs(position))
        hit(*u);
  }

  // only one hit
  void Projectile::hit(Unit& u)
  {
    u.receiveAttack(damage, velocity * inertia);
    explode();
  }

  void Projectile::explode()
  {
    mode = Mode::FADE_OUT;

    Flash* flash = new Flash
    (
        battle, position - 2*velocity, Values::random(30, 50), 0,
//        Values::Color{ 1.0f, 1.0f, 0.6f, 1.0f },
//        Values::Color{ 1.0f, 0.8f, 0.8f, 0.1f },
        Values::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
        Values::Color{ 0.8f, 0.8f, 1.0f, 0.0f },
        30
    );

    battle->add(flash);
  }

  void Projectile::rebound(double normalAngle, double randomAngle)
  {
    if (blockingBehavior == BlockingBehavior::EXPLODE_AND_REBOUND ||
        blockingBehavior == BlockingBehavior::EXPLODE_ONLY)
    {
      explode(); // explode and rebound will need to make a replacement projectile
    }
    if (blockingBehavior == BlockingBehavior::EXPLODE_AND_REBOUND ||
        blockingBehavior == BlockingBehavior::REBOUND_ONLY)
    {
      geometry::Vec2 vel(velocity);
      vel.rotateBy(-normalAngle);
      vel.setRect(-vel.getX(), vel.getY());
      vel.rotateBy(normalAngle + randomAngle);

      if (blockingBehavior == BlockingBehavior::EXPLODE_AND_REBOUND)
        battle->add(new Projectile(battle, team, position, vel, damage, inertia, friendlyFire));
        // should do the renderLength thing by itself so thats cool

      else if (blockingBehavior == BlockingBehavior::REBOUND_ONLY)
        velocity = vel;
    }
  }

  void Projectile::move()
  {
    if (mode == Mode::FLYING)
      Entity::move();
  }

  void Projectile::checkContainment()
  {
    if (!(battle->getBounds().containsAbs(position)))
      explode();
  }

} /* namespace entity */
