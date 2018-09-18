/*
 * TypeSelector.cpp
 *
 *  Created on: Sep 11, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <MouseHandler.h>
#include <Renderer.h>
#include <TypeSelector.h>
#include <UnitLoader.h>
#include <Values.h>
#include <vector>
#include <Battle.h>
#include <GLFW/glfw3.h>

//constexpr Values::Color Values::HIGHLIGHT_COLOR;

namespace gui
{
  const geometry::Vec2 TypeSelector::dx { 0, -BOX_SIZE };

  TypeSelector::TypeSelector(MouseHandler* mouseHandler, geometry::Vec2 start,
      Battle* battle)
  : GUIComponent(mouseHandler, geometry::Box(), Values::BUTTON_COLOR),
    battle(battle),
    units(), types(), boxes(),
    start(start),
    unitAngle(0.0)
  {
    for (int i = 0; i < NUM_TYPES; ++i)
    {
      types.emplace_back(i);

      geometry::Vec2 sideDisplacement(BOX_SIZE/2);
      if (i < BOX_LIMIT_Y)
        sideDisplacement *= -1;

      boxes.emplace_back(start + dx*(i % BOX_LIMIT_Y + 0.5) + sideDisplacement, 0,
          -BOX_SIZE/2, BOX_SIZE/2, -BOX_SIZE/2, BOX_SIZE/2);
    }
    refresh();
  }

  TypeSelector::~TypeSelector()
  {
    for (entity::Unit* u : units)
      delete u;
  }

  void TypeSelector::refresh()
  {
    for (entity::Unit* u : units)
      delete u;
    units.clear();

    for (unsigned int i = 0; i < types.size(); ++i)
    {
      geometry::Vec2 sideDisplacement(BOX_SIZE/2);
      if (i < BOX_LIMIT_Y)
        sideDisplacement *= -1;

      geometry::Vec2 unitPos = start + (i % BOX_LIMIT_Y +0.5)*dx + sideDisplacement;
      units.emplace_back(UnitLoader::make(nullptr, battle->selectedTeam, unitPos, unitAngle,
          types[i]));
    }
  }

  void TypeSelector::update()
  {
    unitAngle += ROTATION_RATE;
    if (unitAngle > Values::PI)
      unitAngle -= Values::TWO_PI;

    for (entity::Unit* u : units)
      u->box.angle = unitAngle;
  }

  void TypeSelector::render(graphics::Renderer& renderer) const
  {
    if (battle->selectedAction == Battle::BattleAction::DELETE)
      return;

    unsigned int selection = static_cast<unsigned int>(
        static_cast<int>(battle->selectedUnitType));

    for (unsigned int i = 0; i < boxes.size(); ++i)
    {
      if (selection == i)
        renderer.addQuad(Values::makeQuad(Values::HIGHLIGHT_COLOR, boxes[i], Values::Depth::UNITS));
      else
        renderer.addQuad(Values::makeQuad(Values::BUTTON_COLOR, boxes[i], Values::Depth::UNITS));
    }

    for (entity::Unit* u : units)
      u->render(renderer);
  }

  bool TypeSelector::handleMouseClick(geometry::Vec2 position, int button,
      int action)
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
      if (!mouseHandler->hasFocus(this))
      {
        bool clicked = false;
        // to activate, we need to click a box
        for (unsigned int i = 0; i < NUM_TYPES; ++i)
        {
          if (boxes[i].containsAbs(position))
          {
            battle->selectedUnitType = static_cast<Battle::UnitType>(
                static_cast<int>(i));
            battle->refreshGUI();
            battle->updateWindowTitle();
            clicked = true;
            break;
          }
        }
        return (clicked && action != GLFW_RELEASE);
      }
      else // already dragging
      {
        // select box whose center is closest to mouse
        double minDist = Values::WINDOW_WIDTH;
        unsigned int closest = 0;
        for (unsigned int i = 0; i < NUM_TYPES; ++i)
        {
          double dist = (position - boxes[i].position).getLength();
          if (dist < minDist)
          {
            minDist = dist;
            closest = i;
          }
        }
        battle->selectedUnitType = static_cast<Battle::UnitType>(
            static_cast<int>(closest));
        battle->refreshGUI();
        battle->updateWindowTitle();
        return (action != GLFW_RELEASE);
      }
    } // end if left mouse button
    return false;
  }

} /* namespace gui */
