/*
 * Battle.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#include <Battle.h>
#include <stddef.h>
#include <Vec2.h>
#include <Window.h>
#include <iterator>
#include <string>


constexpr Values::Color Battle::backgroundColor;

Battle::Battle(Window* window)
: window(window),
  bounds(geometry::Vec2(), geometry::Vec2(Values::BATTLE_WIDTH, Values::BATTLE_HEIGHT)),
  particles(), projectiles(), units(),
  unitLoader(*this),
  selectedTeam(entity::Entity::Team::RED),
  selectedUnitType(UnitType::UNIT),
  selectedAction(BattleAction::SINGLE),
  paused(true)
{
}

Battle::~Battle()
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

void Battle::render(graphics::Renderer& renderer)
{
  renderer.addQuad(Values::makeQuad(backgroundColor, bounds, Values::Depth::BACKGROUND));

  for (auto* p : particles)
    if (p && p->getDepth() < Values::Depth::UNITS)
      p->render(renderer);
  for (auto* p : projectiles)
    if (p && p->getDepth() < Values::Depth::UNITS)
      p->render(renderer);
  for (auto* u : units)
    if (u)
      u->render(renderer);
  for (auto* p : particles)
    if (p && p->getDepth() >= Values::Depth::UNITS)
      p->render(renderer);
  for (auto* p : projectiles)
    if (p && p->getDepth() >= Values::Depth::UNITS)
      p->render(renderer);

  unitLoader.render(renderer);
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

void Battle::resurrectAllIfDead()
{
  for (auto* u : units)
    if (u)
      u->resurrect();
}

void Battle::healAllIfAlive()
{
  for (auto* u : units)
    if (u)
      u->heal();
}

void Battle::add(entity::Projectile* p)
{
  projectiles.emplace_back(p);
}

void Battle::add(entity::Particle* p)
{
  particles.emplace_back(p);
}

void Battle::add(entity::Unit* u)
{
  units.emplace_back(u);
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
      case GLFW_KEY_E:
        selectedUnitType = UnitType::LAUNCHER;
        break;
      case GLFW_KEY_R:
        selectedUnitType = UnitType::CANNON;
        break;
      case GLFW_KEY_T:
        selectedUnitType = UnitType::RAILGUN;
        break;
      case GLFW_KEY_Y:
        selectedUnitType = UnitType::GIANT;
        break;
      case GLFW_KEY_U:
        selectedUnitType = UnitType::SHIELD;
        break;
      case GLFW_KEY_I:
        selectedUnitType = UnitType::RACER;
        break;
      case GLFW_KEY_O:
        selectedUnitType = UnitType::ACE;
        break;

      case GLFW_KEY_A:
        selectedAction = BattleAction::DELETE;
        break;
      case GLFW_KEY_S:
        selectedAction = BattleAction::SINGLE;
        break;
      case GLFW_KEY_D:
        selectedAction = BattleAction::LINE;
        if (unitLoader.isLineStarted())
          unitLoader.cancel();
        break;

      case GLFW_KEY_Z:
        resurrectAllIfDead();
        break;
      case GLFW_KEY_X:
        healAllIfAlive();
        break;
      case GLFW_KEY_C:
        clearAll();
        break;

      case GLFW_KEY_LEFT_BRACKET:
        unitLoader.decrement();
        break;
      case GLFW_KEY_RIGHT_BRACKET:
        unitLoader.increment();
        break;
      case GLFW_KEY_BACKSLASH:
        unitLoader.flip();
        break;
      case GLFW_KEY_ENTER:
        unitLoader.addAndClearAll();
        break;

      default:
        break;
    }

    if (selectedAction != BattleAction::LINE && unitLoader.isLineStarted())
    {
      unitLoader.cancel();
    }

  }
  else if (action == GLFW_REPEAT) // these keys activate on both press and repeat
  {
    switch (key)
    {
      case GLFW_KEY_Z:
        resurrectAllIfDead();
        break;
      case GLFW_KEY_X:
        healAllIfAlive();
        break;

      case GLFW_KEY_LEFT_BRACKET:
        unitLoader.decrement();
        break;
      case GLFW_KEY_RIGHT_BRACKET:
        unitLoader.increment();
        break;

      default:
        break;
    }
  }

  if (selectedAction == BattleAction::LINE)
    unitLoader.refresh();

  updateWindowTitle();

}

bool Battle::handleMouseClick(geometry::Vec2 position, int button, int action)
{
  if (bounds.containsAbs(position))
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
      bool keepFocus = unitLoader.processClick(position);
      updateWindowTitle();
      return keepFocus;
    }
    else if (action == GLFW_PRESS)
    {
      return true;
    }
  }
  return false;
}

void Battle::updateWindowTitle()
{
  std::string msg(" ");

  if (paused)
    msg += "[Paused]";
  else
    msg += "[Running]";

  msg += " Mode: ";
  if (selectedAction == BattleAction::DELETE)
  {
    msg += "Click to delete a unit";
  }
  else if (unitLoader.isLineStarted() &&
      selectedAction == BattleAction::LINE) // special case
  {
    msg += "[ ] to adjust, \\ to flip, Enter to confirm";
  }
  else
  {
    if (selectedAction == BattleAction::SINGLE)
      msg += "Click to place a ";
    else if (selectedAction == BattleAction::LINE)
      msg += "Click twice to place a line of ";

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
        case UnitType::LAUNCHER:
          msg += "launcher";
          break;
        case UnitType::CANNON:
          msg += "cannon";
          break;
        case UnitType::RAILGUN:
          msg += "railgun";
          break;
        case UnitType::GIANT:
          msg += "giant";
          break;
        case UnitType::SHIELD:
          msg += "shield";
          break;
        case UnitType::RACER:
          msg += "racer";
          break;
        case UnitType::ACE:
          msg += "ace";
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
        case UnitType::LAUNCHER:
          msg += "launchers";
          break;
        case UnitType::CANNON:
          msg += "cannons";
          break;
        case UnitType::RAILGUN:
          msg += "railguns";
          break;
        case UnitType::GIANT:
          msg += "giants";
          break;
        case UnitType::SHIELD:
          msg += "shields";
          break;
        case UnitType::RACER:
          msg += "racers";
          break;
        case UnitType::ACE:
          msg += "aces";
          break;
        default:
          msg += "[unknown unit type]s";
          break;
      }
    }
  }

  window->setTitleMessage(msg);
}
