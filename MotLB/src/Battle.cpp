/*
 * Battle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include "Battle.h"

#include <Entity.h>
#include <stddef.h>
#include <Vec2.h>
#include <iterator>
#include <iostream>

#include "Window.h"

constexpr Values::Color Battle::backgroundColor;

Battle::Battle(Window* window)
: window(window),
  bounds(geometry::Vec2(), geometry::Vec2(Values::BATTLE_WIDTH, Values::BATTLE_HEIGHT)),
  particles(), projectiles(), units(),
  renderer(),
  selectedTeam(entity::Entity::Team::RED),
  selectedUnitType(UnitType::UNIT),
  selectedAction(BattleAction::SINGLE),
  paused(true)
{
  if (window)
    window->setBattle(this);
  updateWindowTitle();
}

Battle::~Battle()
{
}

void Battle::run()
{
}

void Battle::stop()
{
}

void Battle::update()
{
  if (paused)
    return;

  for (size_t i = 0; i < units.size(); )
  {
    if (units[i] == nullptr)
    {
      units.erase(units.begin() + i);
    }
    else
    {
      units[i]->update();
      i++;
    }
  }

  for (size_t i = 0; i < projectiles.size(); )
  {
    if (projectiles[i] == nullptr)
    {
      projectiles.erase(projectiles.begin() + i);
    }
    else
    {
      projectiles[i]->update();

      if (!projectiles[i]->isActive())
      {
        delete projectiles[i];
        projectiles.erase(projectiles.begin() + i);
      }
      else
      {
        i++;
      }
    }
  }

  for (size_t i = 0; i < particles.size(); )
  {
    if (particles[i] == nullptr)
    {
      particles.erase(particles.begin() + i);
    }
    else
    {
      particles[i]->update();

      if (!particles[i]->isActive())
      {
        delete particles[i];
        particles.erase(particles.begin() + i);
      }
      else
      {
        i++;
      }
    }
  }
}

void Battle::renderAll()
{
//  Values::CenteredPoly cp =
//  {
//      Values::ColoredVertex{{1.0f, 0.5f, 0.0f, 0.5f}, 400.0f, 400.0f},
//      Values::ColoredVertex{{1.0f, 1.0f, 0.5f, 0.5f}, 800.0f, 800.0f},
//      Values::ColoredVertex{{0.5f, 1.0f, 0.0f, 0.5f}, 800.0f, 0.0f  },
//      Values::ColoredVertex{{1.0f, 1.5f, 0.5f, 0.5f}, 0.0f  , 0.0f  },
//      Values::ColoredVertex{{0.5f, 1.0f, 0.0f, 0.5f}, 0.0f  , 800.0f}
//  };
//
//  renderer.addCenteredPoly(cp);

  renderer.addQuad(Values::makeQuad(backgroundColor, bounds));

  for (auto* p : particles)
    p->render(renderer);
  for (auto* p : projectiles)
    p->render(renderer);
  for (auto* u : units)
    u->render(renderer);

  renderer.renderAndClearAll();
}

void Battle::clearAll()
{
  for (auto* p : particles)
    delete p;
  for (auto* p : projectiles)
    delete p;
  for (auto* u : units)
    delete u;

  particles.clear();
  projectiles.clear();
  units.clear();
}

void Battle::add(entity::Projectile* p)
{
  projectiles.push_back(p);
}

void Battle::add(entity::Particle* p)
{
  particles.push_back(p);
}

void Battle::add(entity::Unit* u)
{
  units.push_back(u);
}

bool Battle::remove(entity::Particle* p)
{
  // std::find?
  for (size_t i = 0; i < particles.size(); i++)
  {
    if (particles[i] == p)
    {
      delete p; // same as delete particles[i]
      particles.erase(particles.begin() + i);
      return true;
    }
  }
  return false;
}

bool Battle::remove(entity::Projectile* p)
{
  for (size_t i = 0; i < projectiles.size(); i++)
  {
    if (projectiles[i] == p)
    {
      delete p;
      projectiles.erase(projectiles.begin() + i);
      return true;
    }
  }
  return false;
}

bool Battle::remove(entity::Unit* u)
{
  for (size_t i = 0; i < units.size(); i++)
  {
    if (units[i] == u)
    {
      delete u;
      units.erase(units.begin() + i);
      return true;
    }
  }
  return false;
}

void Battle::handleKeypress(int key, int action)
{
  // TODO
//  std::cout << "Key " << key << " " << action << std::endl;

  if (action == GLFW_PRESS)
  {
    switch (key)
    {
      case GLFW_KEY_SPACE:
        paused = !paused;
        break;

      case GLFW_KEY_1:
        selectedTeam = entity::Entity::Team::RED;
        break;
      case GLFW_KEY_2:
        selectedTeam = entity::Entity::Team::GREEN;
        break;
      case GLFW_KEY_3:
        selectedTeam = entity::Entity::Team::BLUE;
        break;
      case GLFW_KEY_4:
        selectedTeam = entity::Entity::Team::YELLOW;
        break;

      case GLFW_KEY_Q:
        selectedUnitType = UnitType::UNIT;
        break;
      case GLFW_KEY_W:
        selectedUnitType = UnitType::GUNNER;
        break;

      case GLFW_KEY_A:
        selectedAction = BattleAction::DELETE;
        break;
      case GLFW_KEY_S:
        selectedAction = BattleAction::SINGLE;
        break;
      case GLFW_KEY_D:
        selectedAction = BattleAction::LINE;
        break;

      default:
        break;
    }
  }
//  else if (action == GLFW_RELEASE)
//  {
//    switch (key)
//    {
//      case GLFW_KEY_SPACE:
//        paused = !paused;
//        break;
//
//      default:
//        break;
//    }
//  }

  updateWindowTitle();

}

void Battle::handleMouseClick(int button, int action, double x, double y)
{
  // TODO
//  std::cout << "Mouse " << button << " " << action << " at " << x << ", " << y << std::endl;
}

const geometry::Box& Battle::getBounds() const
{
  return bounds;
}

std::vector<entity::Unit*>& Battle::getUnits()
{
  return units;
}

void Battle::updateWindowTitle()
{
  if (window)
  {
    std::string msg(": ");

    if (paused)
      msg += "[Paused]";
    else
      msg += "[Running]";

    msg += " Mode: ";
    if (selectedAction == BattleAction::DELETE)
    {
      msg += "Click to delete a unit";
    }
    else
    {
      if (selectedAction == BattleAction::SINGLE)
        msg += "Click to place a ";
      else if (selectedAction == BattleAction::LINE)
        msg += "Click twice and scroll to place ";

      switch (selectedTeam)
      {
        case entity::Entity::Team::RED:
          msg += "red ";
          break;
        case entity::Entity::Team::GREEN:
          msg += "green ";
          break;
        case entity::Entity::Team::BLUE:
          msg += "blue ";
          break;
        case entity::Entity::Team::YELLOW:
          msg += "yellow ";
          break;
        default:
          msg += "[unknown team] ";
          break;
      }

      if (selectedAction == BattleAction::SINGLE)
      {
        // singular noun
        switch (selectedUnitType)
        {
          case UnitType::UNIT:
            msg += "unit";
            break;
          case UnitType::GUNNER:
            msg += "gunner";
            break;
          default:
            msg += "[unknown unit type]";
            break;
        }
      }
      else if (selectedAction == BattleAction::LINE)
      {
        // plural noun
        switch (selectedUnitType)
        {
          case UnitType::UNIT:
            msg += "units";
            break;
          case UnitType::GUNNER:
            msg += "gunners";
            break;
          default:
            msg += "[unknown unit types]";
            break;
        }
      }
    }

    window->setTitleMessage(msg);
  }
}
