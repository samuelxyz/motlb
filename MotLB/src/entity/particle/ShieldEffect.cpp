/*
 * ShieldEffect.cpp
 *
 *  Created on: Aug 14, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Entity.h>
#include <particle/ShieldEffect.h>
#include <Renderer.h>
#include <Values.h>
#include <Vec2.h>


namespace entity
{

  ShieldEffect::ShieldEffect(Battle* battle, geometry::Vec2 velocity, geometry::Box& shape, Values::Color startColor, Values::Color endColor,
      unsigned int lifetime)
  : Particle(battle, Entity::Team::NEUTRAL, shape.position, velocity, lifetime, Values::Depth::EMBLEMS),
    box(shape), startColor(startColor), endColor(endColor), timer(0)
  {
  }

  ShieldEffect::~ShieldEffect()
  {
  }

  void ShieldEffect::update()
  {
    ++timer;
    if (timer > lifetime)
    {
      active = false;
      return;
    }
    Entity::update();
  }

  void ShieldEffect::render(graphics::Renderer& renderer) const
  {
    Values::Color color = Values::interpolateColors(
        startColor, endColor, static_cast<double>(timer)/lifetime);
    renderer.addQuad(Values::makeQuad(color, box, depth));
  }

  void ShieldEffect::move()
  {
    box.position += velocity;
  }

} /* namespace entity */
