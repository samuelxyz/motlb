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
      Values::Color start, Values::Color end, float depth)
  : Particle(battle, Entity::Team::NEUTRAL, position, velocity, lifetime, depth),
    startColor(start), endColor(end),
    corners(),
    radius(radius), dr(dr), spin(spin)
  {
    if (radius + (dr * lifetime) < 0)
    {
      lifetime = abs(radius/dr);
    }

    // set up corners
    unsigned int numCorners = 3 + radius/3;
    for (unsigned int i = 0; i < numCorners; ++i)
    {
      double angle = Values::TWO_PI * i/numCorners;
      double randAngle = 0.3 * Values::random(
          -Values::TWO_PI/numCorners, Values::TWO_PI/numCorners);

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
    Particle::update();
    radius += dr;

    for (geometry::Vec2& v : corners)
    {
      v.rotateBy(spin);
      v.scaleTo(radius);
    }
  }

  void Smoke::render(graphics::Renderer& renderer) const
  {
    Values::Color color = Values::interpolateColors(
        startColor, endColor, static_cast<double>(age)/lifetime);
    Values::CenteredPoly cp;

    cp.push_back(Values::makeCV(color, position, depth));
    for (unsigned int i = 0; i < corners.size(); ++i)
    {
      cp.push_back(Values::makeCV(color, position + corners[i], depth));
    }

    renderer.addCenteredPoly(cp);
  }

} /* namespace entity */
