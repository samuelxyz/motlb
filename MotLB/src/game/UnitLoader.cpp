/*
 * UnitLoader.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <Box.h>
#include <Entity.h>
#include <Renderer.h>
#include <unit/Ace.h>
#include <unit/Cannon.h>
#include <unit/Giant.h>
#include <unit/Launcher.h>
#include <unit/Racer.h>
#include <unit/Railgun.h>
#include <unit/Shield.h>

#include <UnitLoader.h>
#include <Values.h>
#include <Window.h>

UnitLoader::UnitLoader(Battle& battle)
: battle(battle), stagedUnits(), count(1),
  alreadyClicked(false), lineStarted(false),
  v1(), v2(), facing()
{
}

UnitLoader::~UnitLoader()
{
  clearStagedUnits();
}

void UnitLoader::render(graphics::Renderer& renderer)
{
  // two modes: clicking for second point, and preview mode

  if (alreadyClicked)
  {
    drawBar(renderer, v1, battle.window->getMousePos());
  }
  else if (lineStarted)
  {
    // draw line/bar thing
    drawBar(renderer, v1, v2);

    // render unit previews
    for (entity::Unit* u : stagedUnits)
    {
      if (u) // make() returns nullptr if battle.selectedUnitType is unknown
        u->render(renderer);
    }

    // draw arrow

    geometry::Vec2 midpoint = (v1 + v2) * 0.5;
    geometry::Vec2 para(v2 - v1);
    geometry::Vec2 perp(facing);
    perp.scaleTo(30);
    para.scaleTo(20);

    Values::Color triColor(
        entity::Entity::getTeamColor(battle.selectedTeam));

    Values::Triangle tri
    {{
      Values::makeCV( triColor, midpoint + perp * 1.5,  Values::Depth::TOP ),
      Values::makeCV( triColor, midpoint + perp + para, Values::Depth::TOP ),
      Values::makeCV( triColor, midpoint + perp - para, Values::Depth::TOP )
    }};

    renderer.addTriangle(tri);
  }
}

void UnitLoader::drawBar(graphics::Renderer& renderer,
    geometry::Vec2 start, geometry::Vec2 end) const
{
  constexpr double halfThickness = 5;
  const Values::Color barColor { 1.0f, 1.0f, 1.0f, 0.3f };

  geometry::Vec2 para(end - start);
  para.scaleTo(halfThickness);
  geometry::Vec2 perp(para);
  perp.rotateBy(Values::HALF_PI);
  perp.scaleTo(halfThickness);

  Values::Quad quad
  {{
    Values::makeCV( barColor, start - para + perp, Values::Depth::BACKGROUND ),
    Values::makeCV( barColor, start - para - perp, Values::Depth::BACKGROUND ),
    Values::makeCV( barColor, end   + para - perp, Values::Depth::BACKGROUND ),
    Values::makeCV( barColor, end   + para + perp, Values::Depth::BACKGROUND )
  }};

  renderer.addQuad(quad);
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
    clearStagedUnits();

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
  if (!lineStarted)
    return;

  // TODO synchronization?
  for (entity::Unit* u : stagedUnits)
  {
    battle.add(u);
  }

  stagedUnits.clear();
  count = 1;
  alreadyClicked = false;
  lineStarted = false;
}

void UnitLoader::cancel()
{
  if (!lineStarted)
    return;

  clearStagedUnits();
  addAndClearAll();
}

void UnitLoader::processClick(geometry::Vec2 position)
{
  if (!battle.bounds.containsAbs(position))
    return;

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
      clearStagedUnits();
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
      return new entity::Unit    (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::GUNNER:
      return new entity::Gunner  (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::LAUNCHER:
      return new entity::Launcher(&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::CANNON:
      return new entity::Cannon  (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::RAILGUN:
      return new entity::Railgun (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::GIANT:
      return new entity::Giant   (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::SHIELD:
      return new entity::Shield  (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::RACER:
      return new entity::Racer   (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
    case Battle::UnitType::ACE:
      return new entity::Ace     (&battle, battle.selectedTeam, position, geometry::Vec2(), angle);
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

void UnitLoader::clearStagedUnits()
{
  // not touching lineStarted or alreadyClicked
  // because this method is called from refresh()

  for (entity::Unit* u : stagedUnits)
  {
    delete u;
  }
  stagedUnits.clear();
}
