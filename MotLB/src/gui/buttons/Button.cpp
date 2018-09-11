/*
 * Button.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#include <Button.h>
#include <MouseHandler.h>
#include <Renderer.h>
#include <GLFW/glfw3.h>

constexpr Values::Color Values::BUTTON_COLOR;

namespace gui
{

  Button::Button(MouseHandler* mouseHandler, const geometry::Box& area,
      Battle* battle)
  : GUIComponent(mouseHandler, area, Values::BUTTON_COLOR), battle(battle)
  {
  }

  Button::~Button()
  {
  }

  void Button::render(graphics::Renderer& renderer) const
  {
    Values::Color buttonColor(color);
    if (mouseHandler->hasFocus(this))
      buttonColor.a = 1.0f;

    renderer.addQuad(Values::makeQuad(buttonColor, box, Values::Depth::UNITS));
  }

  bool Button::handleMouseClick(geometry::Vec2 pos, int button, int action)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_RELEASE)
    {
      if (mouseHandler->hasFocus(this))
        return true;
      else
        return box.containsAbs(pos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE &&
        box.containsAbs(pos))
    {
      clickAction();
    }
    return false;
  }

} /* namespace gui */
