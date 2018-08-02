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
  renderBox(backgroundColor, bounds);

  for (auto& u : units)
    u.render();

  renderer.renderAndClearAll();
}


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

void Battle::renderBox(const Values::Color& color, const geometry::Box& box)
{
  renderer.addColoredBox(color, box);
}

std::vector<entity::Unit>& Battle::getUnits()
{
  return units;
}

