/*
 * LineControls.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#include <LineControls.h>
#include <Values.h>
#include <Battle.h>

#include <ConfirmButton.h>
#include <DecrementButton.h>
#include <FlipButton.h>
#include <IncrementButton.h>

namespace gui
{

  LineControls::LineControls(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : GUIComponent(nullptr, area, Values::BUTTON_COLOR),
    battle(battle)
  {
    const double buttonHalfWidth = 0.5 * box.getWidth() / NUM_BUTTONS;
    const double buttonHalfHeight = 0.5 * box.getHeight();

    geometry::Vec2 dx(buttonHalfWidth * 2);
    dx.rotateBy(box.angle);

    std::array<geometry::Box, NUM_BUTTONS> boxes;
    for (unsigned int i = 0; i < NUM_BUTTONS; ++i)
    {
      boxes[i] = geometry::Box(
          box.position + (i+0.5 - NUM_BUTTONS/2.0) * dx,
          box.angle, -buttonHalfWidth, buttonHalfWidth,
          -buttonHalfHeight, buttonHalfHeight
      );
    }

    children.emplace_back(new IncrementButton(mouseHandler, boxes[0], battle));
    children.emplace_back(new DecrementButton(mouseHandler, boxes[1], battle));
    children.emplace_back(new FlipButton     (mouseHandler, boxes[2], battle));
    children.emplace_back(new ConfirmButton  (mouseHandler, boxes[3], battle));
  }

  LineControls::~LineControls()
  {
  }

  void LineControls::render(graphics::Renderer& renderer) const
  {
    if (battle->getUnitLoader().isLineStarted())
      for (GUIComponent* c : children)
        c->render(renderer);
  }

} /* namespace gui */
