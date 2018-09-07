/*
 * Game.cpp
 *
 *  Created on: Aug 1, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Cannon.h>
#include <Entity.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GUIComponent.h>
#include <Launcher.h>
#include <Railgun.h>
#include <Vec2.h>

#include "Battle.h"
#include "Values.h"
#include "Window.h"

int run()
{
  Window window;

  Battle battle(&window);

  entity::Unit* u1 = new entity::Unit(&battle, entity::Entity::Team::BLUE, geometry::Vec2(600, 600), geometry::Vec2(), 1);
  battle.add(u1);
  entity::Unit* u2 = new entity::Unit(&battle, entity::Entity::Team::RED, geometry::Vec2(200, 200), geometry::Vec2(), 4);
  battle.add(u2);
  entity::Unit* u3 = new entity::Unit(&battle, entity::Entity::Team::GREEN, geometry::Vec2(200, 600), geometry::Vec2(), 2);
  battle.add(u3);

  for (int i = 0; i < 6; i++)
  {
    entity::Unit* ul = new entity::Unit(&battle, entity::Entity::Team::RED, geometry::Vec2(100, 100 + 100 * i), geometry::Vec2(), 0);
    battle.add(ul);
  }

  entity::Gunner* u4 = new entity::Gunner(&battle, entity::Entity::Team::YELLOW, geometry::Vec2(400, 400), geometry::Vec2(), 5);
  battle.add(u4);

//  entity::Projectile* p1 = new entity::Projectile(&battle, entity::Entity::Team::YELLOW,
//      geometry::Vec2(800, 605), geometry::Vec2(-2, 0), 10, 5, true);
//  battle.add(p1);

//  entity::Missile* p2 = new entity::Missile(&battle, entity::Entity::Team::YELLOW,
//      geometry::Vec2(800, 605), geometry::Vec2(-3, 0), 10, 5, u3);
//  battle.add(p2);

  entity::Launcher* u5 = new entity::Launcher(&battle, entity::Entity::Team::BLUE, geometry::Vec2(600, 200), geometry::Vec2(), 2);
  battle.add(u5);

  entity::Cannon* u6 = new entity::Cannon(&battle, entity::Entity::Team::GREEN, geometry::Vec2(20, 780), geometry::Vec2(), 4.5);
  battle.add(u6);

  entity::Railgun* u7 = new entity::Railgun(&battle, entity::Entity::Team::BLUE, geometry::Vec2(20, 20), geometry::Vec2(), 1);
  battle.add(u7);

  while (!window.shouldClose())
  {
    battle.update();
    battle.renderAll();

    window.swapBuffers();

    glfwPollEvents();
  }

  return 0;
}
