/*
 * Projectile.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#include "../Battle.h"
#include "Projectile.h"
#include <array>

namespace entity
{

  Projectile::Projectile(Battle* battle, Team team, geometry::Vec2 position,
      geometry::Vec2 velocity, double damage, double inertia, double friendlyFire)
  : Entity(battle, team, position, velocity),
    damage(damage), inertia(inertia),
    friendlyFire(friendlyFire),
    mode(Mode::FLYING),
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

    checkFading();
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
    Values::Color tailColor { 1.0f, 1.0f, 1.0f, 0.5f };

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
        quad[i] = { tailColor, static_cast<float>(corners[i].getX()),
            static_cast<float>(corners[i].getY()) };
      else
        quad[i] = { bodyColor, static_cast<float>(corners[i].getX()),
            static_cast<float>(corners[i].getY()) };
    }

    renderer.addQuad(quad);

  }

  void Projectile::checkFading()
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
    for (Unit u : battle->getUnits())
      if (u.active && (friendlyFire || u.team != team) &&
          u.box.containsAbs(position))
        hit(u);
  }

  // only one hit
  void Projectile::hit(Unit& u)
  {
    u.receiveAttack(damage, velocity * inertia);
    mode = Mode::FADE_OUT;
  }

  void Projectile::move()
  {
    if (mode == Mode::FLYING)
      Entity::move();
  }

  void Projectile::checkContainment()
  {
    if (!((*battle).getBounds().containsAbs(position)))
      mode = Mode::FADE_OUT;
  }

} /* namespace entity */
