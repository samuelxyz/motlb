/*
 * DecrementButton.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <DecrementButton.h>
#include <Renderer.h>
#include <UnitLoader.h>
#include <Values.h>

namespace gui
{

  DecrementButton::DecrementButton(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  DecrementButton::~DecrementButton()
  {
  }

  void DecrementButton::render(graphics::Renderer& renderer) const
  {
    Button::render(renderer);

    constexpr double scale = 10;
    geometry::Box bar(box.position, box.angle, -scale, scale, -scale/4, scale/4);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, bar, Values::Depth::EMBLEMS));
  }

  void DecrementButton::clickAction() const
  {
    battle->getUnitLoader().decrement();
  }

} /* namespace gui */
