/*
 * ResurrectButton.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <Renderer.h>
#include <ResurrectButton.h>
#include <Values.h>
#include <Vec2.h>

namespace gui
{

  ResurrectButton::ResurrectButton(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  ResurrectButton::~ResurrectButton()
  {
  }

  void ResurrectButton::render(graphics::Renderer& renderer) const
  {
    Button::render(renderer);

    constexpr double scale = 10;

    geometry::Box bar(box.position - geometry::Vec2(scale*1.5), 0, -scale, scale, -scale/3, scale/3);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
    bar.angle += Values::HALF_PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));

    bar.position += geometry::Vec2(scale*3);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
    bar.angle += Values::HALF_PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
  }

  void ResurrectButton::clickAction() const
  {
    battle->resurrectAllIfDead();
  }

} /* namespace gui */
