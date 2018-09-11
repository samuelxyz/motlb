/*
 * ColorSelector.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <ColorSelector.h>
#include <Entity.h>
#include <MouseHandler.h>
#include <Renderer.h>
#include <Values.h>
#include <GLFW/glfw3.h>

namespace gui
{

  ColorSelector::ColorSelector(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : GUIComponent(mouseHandler, area, Values::BUTTON_COLOR),
    battle(battle), sectionWidth(box.getWidth() / NUM_SECTIONS),
    highlight(box.position, box.angle,
        -sectionWidth/2 - HIGHLIGHT_THICKNESS, sectionWidth/2 + HIGHLIGHT_THICKNESS,
        -box.getHeight()/2 - HIGHLIGHT_THICKNESS, box.getHeight()/2 + HIGHLIGHT_THICKNESS)
  {
    dx.setPolar(sectionWidth, box.angle);
    left.setPolar(-box.getWidth() / 2, box.angle);
    left += box.position;
    for (unsigned int i = 0; i < NUM_SECTIONS; ++i)
    {
      boxes[i] = geometry::Box(left + (0.5+i)*dx, box.angle, -sectionWidth/2, sectionWidth/2,
          -box.getHeight()/2, box.getHeight()/2);
    }
  }

  ColorSelector::~ColorSelector()
  {
  }

  void ColorSelector::render(graphics::Renderer& renderer) const
  {
    if (battle->selectedAction == Battle::BattleAction::DELETE)
      return;

    // selection highlight
    int selectedBox = static_cast<int>(battle->selectedTeam);
    highlight.position = left + (selectedBox - 0.5)*dx;
    renderer.addQuad(Values::makeQuad(color, highlight, Values::Depth::UNITS));

    for (unsigned int i = 0; i < NUM_SECTIONS; ++i)
    {
      renderer.addQuad(Values::makeQuad(
          entity::Entity::getTeamColor(
              static_cast<entity::Entity::Team>(
                  static_cast<int>(i+1))),
          boxes[i],
          Values::Depth::EMBLEMS
      ));
    }
  }

  bool ColorSelector::handleMouseClick(geometry::Vec2 pos, int button,
      int action)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
      if (!mouseHandler->hasFocus(this))
      {
        bool clicked = false;
        // to activate, we need to click a box
        for (unsigned int i = 0; i < NUM_SECTIONS; ++i)
        {
          if (boxes[i].containsAbs(pos))
          {
            battle->selectedTeam = static_cast<entity::Entity::Team>(
                static_cast<int>(i+1));
            battle->updateWindowTitle();
            clicked = true;
            break;
          }
        }
        return clicked;
      }
      else // we're dragging already
      {
        geometry::Vec2 rayToMouse = pos - left;

        // get component parallel to box row
        rayToMouse.rotateBy(-box.angle);
        double dist = rayToMouse.x / sectionWidth;

        int selection;
        if (dist < 0)
          selection = 1;
        else if (dist >= NUM_SECTIONS)
          selection = NUM_SECTIONS;
        else
          selection = static_cast<int>(dist) + 1;

        battle->selectedTeam = static_cast<entity::Entity::Team>(selection);
        battle->updateWindowTitle();

        return (action != GLFW_RELEASE);
      }
    }
    return false;
  }

} /* namespace gui */
