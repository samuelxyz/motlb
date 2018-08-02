/*
 * GameTest.cpp
 *
 *  Created on: Aug 1, 2018
 *      Author: Samuel Tan
 */

#include "Battle.h"
#include "entity/Unit.h"
#include "Window.h"

int battleRenderTest()
{
  Window window;

  Battle battle;

  entity::Unit u1(&battle, entity::Entity::Team::BLUE, geometry::Vec2(600, 600), geometry::Vec2(), 1);
  battle.add(u1);
  entity::Unit u2(&battle, entity::Entity::Team::RED, geometry::Vec2(200, 200), geometry::Vec2(), 4);
  battle.add(u2);
  entity::Unit u3(&battle, entity::Entity::Team::GREEN, geometry::Vec2(200, 600), geometry::Vec2(), 2);
  battle.add(u3);

  while (!window.shouldClose())
  {
    battle.update();
    battle.renderAll();

    window.swapBuffers();

    glfwPollEvents();
  }

  return 0;
}
