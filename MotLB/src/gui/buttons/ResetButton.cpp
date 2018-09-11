/*
 * ResetButton.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <Renderer.h>
#include <ResetButton.h>
#include <Values.h>

namespace gui
{

  ResetButton::ResetButton(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  ResetButton::~ResetButton()
  {
  }

  void ResetButton::render(graphics::Renderer& renderer) const
  {
    Button::render(renderer);

    constexpr double scale = 12;
    geometry::Box bar(box.position, Values::HALF_PI/2, -scale, scale, -scale/4, scale/4);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
    bar.angle += Values::HALF_PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
  }

  void ResetButton::clickAction() const
  {
    battle->clearAll();
  }

} /* namespace gui */
