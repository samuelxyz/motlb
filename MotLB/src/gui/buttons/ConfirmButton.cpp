/*
 * ConfirmButton.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <ConfirmButton.h>
#include <Renderer.h>
#include <UnitLoader.h>
#include <Values.h>
#include <Vec2.h>

namespace gui
{

  ConfirmButton::ConfirmButton(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  ConfirmButton::~ConfirmButton()
  {
  }

  void ConfirmButton::render(graphics::Renderer& renderer) const
  {
    Button::render(renderer);

    constexpr double scale = 10;
    const geometry::Vec2 hingePoint(box.position + geometry::Vec2(-scale/3, -scale*0.7));

    geometry::Box left(hingePoint, Values::HALF_PI*3/2, 0, scale, -scale/2, 0);
    geometry::Box right(hingePoint, Values::HALF_PI/3, 0, scale*2, 0, scale/2);

    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, left, Values::Depth::EMBLEMS));
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, right, Values::Depth::EMBLEMS));

  }

  void ConfirmButton::clickAction() const
  {
    battle->getUnitLoader().addAndClearAll();
  }

} /* namespace gui */
