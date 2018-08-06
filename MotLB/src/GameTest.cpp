/*
 * GameTest.cpp
 *
 *  Created on: Aug 1, 2018
 *      Author: Samuel Tan
 */

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <Unit.h>
#include <Vec2.h>

#include "Battle.h"
#include "Window.h"
#include "entity/Projectile.h"

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

  entity::Projectile p1(&battle, entity::Entity::Team::YELLOW,
      geometry::Vec2(800, 605), geometry::Vec2(-2, 0), 10, 5, true);
  battle.add(p1);

  while (!window.shouldClose())
  {
    battle.update();
    battle.renderAll();

    window.swapBuffers();

    glfwPollEvents();
  }

  return 0;
}
