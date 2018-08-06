/*
 * Battle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include "Battle.h"

#include <stddef.h>
#include <iterator>

#include "geometry/Vec2.h"
#include "Values.h"

constexpr Values::Color Battle::backgroundColor;

Battle::Battle()
  : bounds(geometry::Vec2(), geometry::Vec2(Values::BATTLE_WIDTH, Values::BATTLE_HEIGHT)),
    particles(), projectiles(), units(),
    renderer()
{
}

Battle::~Battle()
{
}

void Battle::run()
{
}

void Battle::stop()
{
}

void Battle::update()
{
  for (auto& u : units)
    u.update();
  for (auto& p : projectiles)
    p.update();
  for (auto& p : particles)
    p.update();
}

void Battle::renderAll()
{
//  Values::CenteredPoly cp =
//  {
//      Values::ColoredVertex{{1.0f, 0.5f, 0.0f, 0.5f}, 400.0f, 400.0f},
//      Values::ColoredVertex{{1.0f, 1.0f, 0.5f, 0.5f}, 800.0f, 800.0f},
//      Values::ColoredVertex{{0.5f, 1.0f, 0.0f, 0.5f}, 800.0f, 0.0f  },
//      Values::ColoredVertex{{1.0f, 1.5f, 0.5f, 0.5f}, 0.0f  , 0.0f  },
//      Values::ColoredVertex{{0.5f, 1.0f, 0.0f, 0.5f}, 0.0f  , 800.0f}
//  };
//
//  renderer.addCenteredPoly(cp);

  renderer.addQuad(Values::makeQuad(backgroundColor, bounds));

  for (auto& u : units)
    u.render(renderer);

  renderer.renderAndClearAll();
}

//void Battle::renderCP()
//{
//  Values::CenteredPoly cp =
//  {
//      Values::ColoredVertex{{1.0f, 0.5f, 0.0f, 1.0f}, 400.0f, 400.0f},
//      Values::ColoredVertex{{1.0f, 1.0f, 0.5f, 1.0f}, 800.0f, 800.0f},
//      Values::ColoredVertex{{0.5f, 1.0f, 0.0f, 1.0f}, 800.0f, 0.0f  },
//      Values::ColoredVertex{{1.0f, 1.5f, 0.5f, 1.0f}, 0.0f  , 0.0f  },
//      Values::ColoredVertex{{0.5f, 1.0f, 0.0f, 1.0f}, 0.0f  , 800.0f}
//  };
//
//  renderer.addCenteredPoly(cp);
//}

void Battle::clearAll()
{
  particles.clear();
  projectiles.clear();
  units.clear();
}

geometry::Box const& Battle::getBounds() const
{
  return bounds;
}

void Battle::add(entity::Projectile& p)
{
  projectiles.push_back(p);
}

void Battle::add(entity::Particle& p)
{
  particles.push_back(p);
}

void Battle::add(entity::Unit& u)
{
  units.push_back(u);
}

void Battle::remove(entity::Particle& particle)
{
  // std::find?
  for (size_t i = 0; i < particles.size(); i++)
  {
    if (&(particles[i]) == &particle)
    {
      particles.erase(particles.begin() + i);
      return;
    }
  }
}

void Battle::remove(entity::Projectile& p)
{
  for (size_t i = 0; i < projectiles.size(); i++)
    if (&projectiles[i] == &p)
    {
      projectiles.erase(projectiles.begin() + i);
      return;
    }
}

void Battle::remove(entity::Unit& u)
{
  for (size_t i = 0; i < units.size(); i++)
    if (&units[i] == &u)
    {
      units.erase(units.begin() + i);
      return;
    }
}

std::vector<entity::Unit>& Battle::getUnits()
{
  return units;
}

