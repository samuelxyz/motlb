/*
 * Window.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Cannon.h>
#include <Entity.h>
#include <Launcher.h>
#include <Railgun.h>
#include <Vec2.h>
#include <Window.h>
#include <cassert>

#ifdef MOTLB_DEBUG
#include <cstdio>
#include <iostream>
#endif

#include <Battle.h>
#include <Values.h>

Window::Window() : Window(Values::WINDOW_WIDTH, Values::WINDOW_HEIGHT, "MotLB", nullptr)
{
}

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor)
: initializerDummy(width, height, title, monitor),
  window(initializerDummy.window),
  title(title),
  renderer(),
  sidePanel(
      geometry::Box(geometry::Vec2(Values::BATTLE_WIDTH, 0),
          geometry::Vec2(Values::WINDOW_WIDTH, Values::SIDE_PANEL_HEIGHT)),
      Values::Color { 0.0f, 0.08f, 0.13f, 1.0f }
  ),
  topPanel(
      geometry::Box(geometry::Vec2(0, Values::BATTLE_HEIGHT),
          geometry::Vec2(Values::WINDOW_WIDTH, Values::WINDOW_HEIGHT)),
      Values::Color { 0.0f, 0.08f, 0.13f, 1.0f }
  ),
  battle(this)
{

  gui::GUIComponent* sideBorder(new gui::GUIComponent(
      geometry::Box(geometry::Vec2(Values::BATTLE_WIDTH, 0),
          geometry::Vec2(Values::BATTLE_WIDTH + Values::BORDER_THICKNESS, Values::SIDE_PANEL_HEIGHT)),
      Values::Color { 0.6f, 0.6f, 1.0f, 0.6f }
  ));

  gui::GUIComponent* topBorder(new gui::GUIComponent(
      geometry::Box(geometry::Vec2(0, Values::BATTLE_HEIGHT),
          geometry::Vec2(Values::WINDOW_WIDTH, Values::BATTLE_HEIGHT + Values::BORDER_THICKNESS)),
      Values::Color { 0.6f, 0.6f, 1.0f, 0.6f }
  ));

  sidePanel.addChild(sideBorder);
  topPanel.addChild(topBorder);

//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LEQUAL);
  battle.updateWindowTitle();
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
}

Window::~Window()
{
  glfwTerminate();
}

void Window::update()
{
  battle.update();
}

void Window::render()
{
  battle.render(renderer);
  sidePanel.render(renderer);
  topPanel.render(renderer);
  renderer.renderAndClearAll();
  glfwSwapBuffers(window);
}

bool Window::shouldClose() const
{
  return glfwWindowShouldClose(window);
}

void Window::handleKey(GLFWwindow* window, int key, int scancode, int action,
    int mods)
{
 ((Window*)glfwGetWindowUserPointer(window))->battle.handleKeypress(key, action);
}

void Window::handleMouseButton(GLFWwindow* window, int button, int action,
    int mods)
{
  ((Window*)glfwGetWindowUserPointer(window))->battle.handleMouseClick(button, action);
}

geometry::Vec2 Window::getMousePos() const
{
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  y = Values::WINDOW_HEIGHT - y;
  return geometry::Vec2(x, y);
}

void Window::setTitleMessage(const std::string& msg)
{
  glfwSetWindowTitle(window, (title + msg).c_str());
}

#ifdef MOTLB_DEBUG /////////////////////////

  void Window::printGLFWError(int error, const char* description)
  {
    fprintf(stderr, "[GLFW Error] %s\n", description);
  }

  void APIENTRY Window::printGLDebug(GLenum source, GLenum type, GLuint id,
      GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
  {
    //  std::cout << "[GL][" << __FILE__ << ": line " << __LINE__ << "] " << message << std::endl;
    std::cout << "[GL] " << message << std::endl;
}

#endif /////////////////////////////////////