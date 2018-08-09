/*
 * UnitLoader.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: Samuel Tan
 */

#include "UnitLoader.h"

#include <Box.h>
#include <Entity.h>
#include <Gunner.h>
#include <Renderer.h>
#include <Vec2.h>
#include <array>

#include "Values.h"
#include "Battle.h"

UnitLoader::UnitLoader(Battle& battle)
: battle(battle), stagedUnits(), count(1),
  alreadyClicked(false), lineStarted(false),
  v1(), v2(), facing()
{
}

UnitLoader::~UnitLoader()
{
  clearAll();
}

void UnitLoader::render(graphics::Renderer& renderer)
{
  for (entity::Unit* u : stagedUnits)
  {
    if (u) // make() returns nullptr if battle.selectedUnitType is unknown
      u->render(renderer);
  }

  // draw arrow

  geometry::Vec2 midpoint = (v1 + v2) * 0.5;
  geometry::Vec2 perp(facing);
  perp.scaleTo(30);
  geometry::Vec2 para(perp);
  para.scaleTo(20);
  para.rotateBy(Values::HALF_PI);

  std::array<geometry::Vec2, 3> corners
  {
    midpoint + 1.5*perp,
    midpoint + perp + para,
    midpoint + perp - para
  };

  Values::Triangle tri;
  for (unsigned int i = 0; i < 3; ++i)
  {
    tri[i] =
    {
        entity::Entity::getTeamColor(battle.selectedTeam),
        static_cast<float>(corners[i].getX()),
        static_cast<float>(corners[i].getY())
    };
  }

  renderer.addTriangle(tri);
}

void UnitLoader::flip()
{
  facing *= -1;
  refresh();
}

void UnitLoader::increment()
{
  if (battle.selectedAction == Battle::BattleAction::LINE)
  {
    ++count;
    refresh();
  }
}

void UnitLoader::decrement()
{
  if (battle.selectedAction == Battle::BattleAction::LINE &&
      count > 0)
  {
    --count;
    refresh();
  }
}

void UnitLoader::refresh()
{
  if (stagedUnits.size())
    clearAll();

  if (count == 0)
    return;

  if (count == 1) // special case
  {
    geometry::Vec2 pos((v2 + v1) * 0.5);
    double angle = facing.getAngle();

    stagedUnits.push_back(make(pos, angle));
  }
  else
  {
    geometry::Vec2 step(v2 - v1);
    step *= 1.0/(count-1);

    geometry::Vec2 pos(v1);

    for (unsigned int i = 0; i < count; ++i)
    {
      stagedUnits.push_back(make(pos, facing.getAngle()));
      pos += step;
    }
  }
}

void UnitLoader::addAndClearAll()
{
  // TODO synchronization?
  for (entity::Unit* u : stagedUnits)
  {
    battle.add(u);
  }
  clearAll();
  alreadyClicked = false;
  lineStarted = false;
}

void UnitLoader::clearAll()
{
  // not touching lineStarted or alreadyClicked
  // because this method is called from refresh()

  for (entity::Unit* u : stagedUnits)
  {
    delete u;
  }
  stagedUnits.clear();
}

void UnitLoader::processClick(geometry::Vec2 position)
{
  if (battle.selectedAction == Battle::BattleAction::LINE)
  {
    if (alreadyClicked)
    {
//      lineStarted = true; // redundant, is set in startLine()
      alreadyClicked = false;
      startLine(v1, position);
    }
    else
    {
      clearAll();
      lineStarted = false;
      alreadyClicked = true;
      v1 = position;
    }
  }
  else
  {
    lineStarted = false;
    alreadyClicked = false;
    if (battle.selectedAction == Battle::BattleAction::SINGLE)
    {
      battle.add(make(position));
    }
    else if (battle.selectedAction == Battle::BattleAction::DELETE)
    {
      for (entity::Unit* u : battle.units)
      {
        if (u->getBox().containsAbs(position))
        {
          battle.remove(u);
          break;
        }
      }
    }
  }
}

bool UnitLoader::isLineStarted()
{
  return lineStarted;
}

entity::Unit* UnitLoader::make(geometry::Vec2 position)
{
  double angle = (-battle.bounds.toClosestEdge(position)).getAngle();
  return make(position, angle);
}

entity::Unit* UnitLoader::make(geometry::Vec2 position, double angle)
{
  switch (battle.selectedUnitType)
  {
    case Battle::UnitType::UNIT:
      return new entity::Unit(&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::GUNNER:
      return new entity::Gunner(&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    default:
      return nullptr; // battle can handle this
  }
}

void UnitLoader::startLine(geometry::Vec2 start, geometry::Vec2 end)
{
  lineStarted = true;
  v1 = start;
  v2 = end;

  facing = v2 - v1;
  facing.rotateBy(-Values::HALF_PI);

  count = 1;

  refresh();
}
