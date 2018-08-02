/*
 * Battle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include "Battle.h"

#include "Vec2.h"

Battle::Battle()
  : bounds(Vec2(-1, -1), Vec2(1, 1)), // TODO projection matrices
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

Box const& Battle::getBounds() const
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

void Battle::renderBox(float r, float g, float b, float a, Box& box)
{
  renderer.addColoredBox(r, g, b, a, box);
}

std::vector<entity::Unit>& Battle::getUnits()
{
  return units;
}

