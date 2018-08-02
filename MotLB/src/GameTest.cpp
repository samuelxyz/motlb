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
  entity::Unit testUnit(&battle, entity::Entity::Team::BLUE, Vec2(.01, .01), Vec2(), 0);
  battle.add(testUnit);

  while (!window.shouldClose())
  {
    battle.update();
    battle.renderAll();

    window.swapBuffers();

    glfwPollEvents();
  }

  return 0;
}
