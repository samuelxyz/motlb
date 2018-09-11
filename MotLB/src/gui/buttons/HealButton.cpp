/*
 * HealButton.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <HealButton.h>
#include <Renderer.h>
#include <Values.h>

namespace gui
{

  HealButton::HealButton(MouseHandler* mouseHandler, const geometry::Box& area,
      Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  HealButton::~HealButton()
  {
  }

  void HealButton::render(graphics::Renderer& renderer) const
  {
    Button::render(renderer);

    constexpr double scale = 10;
    geometry::Box bar(box.position, 0, -scale, scale, -scale/3, scale/3);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
    bar.angle += Values::HALF_PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
  }

  void HealButton::clickAction() const
  {
    battle->healAllIfAlive();
  }

} /* namespace gui */
