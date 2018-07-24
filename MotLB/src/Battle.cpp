/*
 * Battle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include "Battle.h"

#include "Vec2.h"

Battle::Battle()
  : bounds(Vec2(), Vec2(800, 800))
{
  // TODO Auto-generated constructor stub

}

Battle::~Battle()
{
  // TODO Auto-generated destructor stub
}

void Battle::run()
{
}

void Battle::stop()
{
}

template <class... Args>
void Battle::add(entity::Entity::Type type, Args&&... args)
{
  switch (type)
  {
    case entity::Entity::Type::PARTICLE:
      particles.emplace_back(args);
      break;
    case entity::Entity::Type::PROJECTILE:
      projectiles.emplace_back(args);
      break;
    case entity::Entity::Type::UNIT:
      units.emplace_back(args);
      break;
  }
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

void Battle::update()
{
  for (auto& u : units)
    u.update();
  for (auto& p : projectiles)
    p.update();
  for (auto& p : particles)
    p.update();
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

void Battle::render()
{
}
