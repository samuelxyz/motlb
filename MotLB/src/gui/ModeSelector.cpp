/*
 * ModeSelector.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <Box.h>
#include <Entity.h>
#include <ModeSelector.h>
#include <MouseHandler.h>
#include <Renderer.h>
#include <Values.h>
#include <GLFW/glfw3.h>

namespace gui
{

  ModeSelector::ModeSelector(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : GUIComponent(mouseHandler, area, Values::BUTTON_COLOR),
    battle(battle),
    sectionWidth(area.getWidth()/NUM_MODES)
  {
    dx.setPolar(sectionWidth, box.angle);
    left.setPolar(-box.getWidth() / 2, box.angle);
    left += box.position;
    for (unsigned int i = 0; i < NUM_MODES; ++i)
    {
      boxes[i] = geometry::Box(left + (0.5+i)*dx, box.angle, -sectionWidth/2, sectionWidth/2,
          -box.getHeight()/2, box.getHeight()/2);
    }
  }

  ModeSelector::~ModeSelector()
  {
  }

  void ModeSelector::render(graphics::Renderer& renderer) const
  {
    Values::Color highlightColor = color;
    highlightColor.a = 1.0f;
    unsigned int selection = static_cast<unsigned int>(
        static_cast<int>(battle->selectedAction));

    GUIComponent::render(renderer);
    renderer.addQuad(Values::makeQuad(highlightColor, boxes[selection], Values::Depth::UNITS));

    constexpr double scale = 12;
    geometry::Box deleteBar(boxes[0].position, box.angle + Values::HALF_PI/2, -scale, scale, -scale/4, scale/4);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, deleteBar, Values::Depth::EMBLEMS));
    deleteBar.angle += Values::HALF_PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, deleteBar, Values::Depth::EMBLEMS));

    constexpr double plusScale = 10;
    geometry::Box plusBar(boxes[1].position, box.angle, -plusScale, plusScale, -plusScale/4, plusScale/4);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, plusBar, Values::Depth::EMBLEMS));
    plusBar.angle += Values::HALF_PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, plusBar, Values::Depth::EMBLEMS));

    geometry::Box lineVertLeft(boxes[2].position, box.angle, -plusScale/2 - plusScale, -plusScale, -plusScale, plusScale);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, lineVertLeft, Values::Depth::EMBLEMS));
    geometry::Box lineVertRight(boxes[2].position, box.angle, -plusScale/2 + plusScale, plusScale, -plusScale, plusScale);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, lineVertRight, Values::Depth::EMBLEMS));
    geometry::Box lineHoriz(boxes[2].position, box.angle, -plusScale, plusScale, -plusScale/4, plusScale/4);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, lineHoriz, Values::Depth::EMBLEMS));
  }

  bool ModeSelector::handleMouseClick(geometry::Vec2 pos, int button,
      int action)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
          if (!mouseHandler->hasFocus(this))
          {
            bool clicked = false;
            // to activate, we need to click a box
            for (unsigned int i = 0; i < NUM_MODES; ++i)
            {
              if (boxes[i].containsAbs(pos))
              {
                battle->selectedAction = static_cast<Battle::BattleAction>(
                    static_cast<int>(i));
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
              selection = 0;
            else if (dist >= NUM_MODES)
              selection = NUM_MODES - 1;
            else
              selection = static_cast<int>(dist);

            battle->selectedAction = static_cast<Battle::BattleAction>(selection);
            battle->updateWindowTitle();

            return (action != GLFW_RELEASE);
          }
        }
        return false;
  }

} /* namespace gui */
