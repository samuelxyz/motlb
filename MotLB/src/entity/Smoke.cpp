/*
 * Smoke.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#include <Smoke.h>
#include <Vec2.h>
#include <Renderer.h>

namespace entity
{

  Smoke::Smoke(Battle* battle, geometry::Vec2 position, geometry::Vec2 velocity,
      double radius, double dr, double spin, unsigned int lifetime,
      Values::Color start, Values::Color end)
  : Particle(battle, Entity::Team::NEUTRAL, position, velocity, radius, lifetime),
    start(start), end(end),
    corners(),
    age(0),
    spin(spin)
  {
    // some initialization stuffs, meh
    this->dr = dr;

    if (radius + (dr * lifetime) < 0)
    {
      lifetime = abs(radius/dr);
    }

    // set up corners
    unsigned int numCorners = 3 + radius/3;
    for (unsigned int i = 0; i < numCorners; ++i)
    {
      double angle = Values::TWO_PI * i/numCorners;
      double randAngle = Values::random(-Values::TWO_PI/numCorners, Values::TWO_PI/numCorners) * 0.3;

      geometry::Vec2 v;
      v.setPolar(radius, angle + randAngle);
      corners.push_back(v);
    }
  }

  Smoke::~Smoke()
  {
  }

  void Smoke::update()
  {
    ++age;
    if (age > lifetime)
    {
      active = false;
      return;
    }

    // updates radius with dr
    Particle::update();

    for (geometry::Vec2& v : corners)
    {
      v.rotateBy(spin);
      v.scaleTo(radius);
    }
  }

  void Smoke::render(graphics::Renderer& renderer) const
  {
    Values::Color color = Values::interpolateColors(start, end, static_cast<double>(age)/lifetime);
    Values::CenteredPoly cp;

    cp.push_back(Values::makeCV(color, position));
    for (unsigned int i = 0; i < corners.size(); ++i)
    {
      cp.push_back(Values::makeCV(color, position + corners[i]));
    }

    renderer.addCenteredPoly(cp);
  }

} /* namespace entity */
