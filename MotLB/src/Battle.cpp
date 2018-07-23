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

void Battle::add(Entity& e)
{
}

void Battle::remove(Entity& e)
{
}

void Battle::clearAll()
{
}

Box const& Battle::getBounds() const
{
  return bounds;
}

void Battle::update()
{
}

void Battle::remove(Particle& particle)
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

void Battle::add(Particle& p)
{
  particles.push_back(p);
}
