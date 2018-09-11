/*
 * Window.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <ModeSelector.h>
#include <ColorSelector.h>
#include <HealButton.h>
#include <ResetButton.h>
#include <ResurrectButton.h>
#include <StartStopButton.h>
#include <Values.h>
#include <Vec2.h>
#include <Window.h>

#ifdef MOTLB_DEBUG
#include <cstdio>
#include <iostream>
#endif

/*static*/ constexpr Values::Color Values::PANEL_COLOR, Values::BORDER_COLOR;

Window::Window() : Window(Values::WINDOW_WIDTH, Values::WINDOW_HEIGHT, "MotLB", nullptr)
{
}

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor)
: initializerDummy(width, height, title, monitor),
  window(initializerDummy.window),
  title(title),
  renderer(),
  mouseHandler(),
  sidePanel(
      nullptr,
      geometry::Box(geometry::Vec2(Values::BATTLE_WIDTH, 0),
          geometry::Vec2(Values::WINDOW_WIDTH, Values::SIDE_PANEL_HEIGHT)),
      Values::PANEL_COLOR
  ),
  topPanel(
      nullptr,
      geometry::Box(geometry::Vec2(0, Values::BATTLE_HEIGHT),
          geometry::Vec2(Values::WINDOW_WIDTH, Values::WINDOW_HEIGHT)),
      Values::PANEL_COLOR
  ),
  battle(this)
{
  mouseHandler.addReceiver(&battle);

  gui::GUIComponent* sideBorder(new gui::GUIComponent(
      nullptr,
      geometry::Box(geometry::Vec2(Values::BATTLE_WIDTH, 0),
          geometry::Vec2(Values::BATTLE_WIDTH + Values::BORDER_THICKNESS, Values::SIDE_PANEL_HEIGHT)),
      Values::BORDER_COLOR
  ));

  gui::GUIComponent* topBorder(new gui::GUIComponent(
      nullptr,
      geometry::Box(geometry::Vec2(0, Values::BATTLE_HEIGHT),
          geometry::Vec2(Values::WINDOW_WIDTH, Values::BATTLE_HEIGHT + Values::BORDER_THICKNESS)),
      Values::BORDER_COLOR
  ));

  sidePanel.addChild(sideBorder);
  topPanel.addChild(topBorder);

  // buttons
  constexpr double BUTTON_WIDTH = 80;

  constexpr double startStopRight = Values::BUTTON_PADDING + BUTTON_WIDTH;
  gui::GUIComponent* startStop(new gui::StartStopButton(&mouseHandler,
      geometry::Box(
          geometry::Vec2(Values::BUTTON_PADDING, Values::BATTLE_HEIGHT + Values::BUTTON_PADDING),
          geometry::Vec2(startStopRight, Values::WINDOW_HEIGHT - Values::BUTTON_PADDING)
      ),
      &battle
  ));
  topPanel.addChild(startStop);

  constexpr double resurrectRight = startStopRight + Values::BUTTON_PADDING + BUTTON_WIDTH;
  gui::GUIComponent* resurrect(new gui::ResurrectButton(&mouseHandler,
      geometry::Box(
          geometry::Vec2(startStopRight + Values::BUTTON_PADDING, Values::BATTLE_HEIGHT + Values::BUTTON_PADDING),
          geometry::Vec2(resurrectRight, Values::WINDOW_HEIGHT - Values::BUTTON_PADDING)
      ),
      &battle
  ));
  topPanel.addChild(resurrect);

  constexpr double healRight = resurrectRight + Values::BUTTON_PADDING + BUTTON_WIDTH;
  gui::GUIComponent* heal(new gui::HealButton(&mouseHandler,
      geometry::Box(
          geometry::Vec2(resurrectRight + Values::BUTTON_PADDING, Values::BATTLE_HEIGHT + Values::BUTTON_PADDING),
          geometry::Vec2(healRight, Values::WINDOW_HEIGHT - Values::BUTTON_PADDING)
      ),
      &battle
  ));
  topPanel.addChild(heal);

  constexpr double resetRight = healRight + Values::BUTTON_PADDING + BUTTON_WIDTH;
  gui::GUIComponent* reset(new gui::ResetButton(&mouseHandler,
      geometry::Box(
          geometry::Vec2(healRight + Values::BUTTON_PADDING, Values::BATTLE_HEIGHT + Values::BUTTON_PADDING),
          geometry::Vec2(resetRight, Values::WINDOW_HEIGHT - Values::BUTTON_PADDING)
      ),
      &battle
  ));
  topPanel.addChild(reset);

  constexpr double modeBottom = Values::SIDE_PANEL_HEIGHT - 2*Values::BUTTON_PADDING;
  gui::GUIComponent* modeSelect(new gui::ModeSelector(&mouseHandler,
      geometry::Box(
          geometry::Vec2(Values::BATTLE_WIDTH + Values::BUTTON_PADDING, Values::SIDE_PANEL_HEIGHT - Values::BUTTON_PADDING),
          geometry::Vec2(Values::WINDOW_WIDTH - Values::BUTTON_PADDING, modeBottom)
      ),
      &battle
  ));
  sidePanel.addChild(modeSelect);

  constexpr double colorBottom = modeBottom - 2*Values::BUTTON_PADDING;
  gui::GUIComponent* colorSelect(new gui::ColorSelector(&mouseHandler,
      geometry::Box(
          geometry::Vec2(Values::BATTLE_WIDTH + Values::BUTTON_PADDING, modeBottom - Values::BUTTON_PADDING),
          geometry::Vec2(Values::WINDOW_WIDTH - Values::BUTTON_PADDING, colorBottom)
      ),
      &battle
  ));
  sidePanel.addChild(colorSelect);

//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LEQUAL);

  battle.updateWindowTitle();

//  entity::Unit* u1 = new entity::Unit(&battle, entity::Entity::Team::BLUE, geometry::Vec2(600, 600), geometry::Vec2(), 1);
//  battle.add(u1);
//  entity::Unit* u2 = new entity::Unit(&battle, entity::Entity::Team::RED, geometry::Vec2(200, 200), geometry::Vec2(), 4);
//  battle.add(u2);
//  entity::Unit* u3 = new entity::Unit(&battle, entity::Entity::Team::GREEN, geometry::Vec2(200, 600), geometry::Vec2(), 2);
//  battle.add(u3);
//
//  for (int i = 0; i < 6; i++)
//  {
//    entity::Unit* ul = new entity::Unit(&battle, entity::Entity::Team::RED, geometry::Vec2(100, 100 + 100 * i), geometry::Vec2(), 0);
//    battle.add(ul);
//  }
//
//  entity::Gunner* u4 = new entity::Gunner(&battle, entity::Entity::Team::YELLOW, geometry::Vec2(400, 400), geometry::Vec2(), 5);
//  battle.add(u4);
//
//  //  entity::Projectile* p1 = new entity::Projectile(&battle, entity::Entity::Team::YELLOW,
//  //      geometry::Vec2(800, 605), geometry::Vec2(-2, 0), 10, 5, true);
//  //  battle.add(p1);
//
//  //  entity::Missile* p2 = new entity::Missile(&battle, entity::Entity::Team::YELLOW,
//  //      geometry::Vec2(800, 605), geometry::Vec2(-3, 0), 10, 5, u3);
//  //  battle.add(p2);
//
//  entity::Launcher* u5 = new entity::Launcher(&battle, entity::Entity::Team::BLUE, geometry::Vec2(600, 200), geometry::Vec2(), 2);
//  battle.add(u5);
//
//  entity::Cannon* u6 = new entity::Cannon(&battle, entity::Entity::Team::GREEN, geometry::Vec2(20, 780), geometry::Vec2(), 4.5);
//  battle.add(u6);
//
//  entity::Railgun* u7 = new entity::Railgun(&battle, entity::Entity::Team::BLUE, geometry::Vec2(20, 20), geometry::Vec2(), 1);
//  battle.add(u7);
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
 ((Window*)glfwGetWindowUserPointer(window))->
     battle.handleKeypress(key, action);
}

void Window::handleMouseButton(GLFWwindow* window, int button, int action,
    int mods)
{
  Window* w = ((Window*)glfwGetWindowUserPointer(window));
  w->mouseHandler.handleMouseClick(w->getMousePos(), button, action);
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
