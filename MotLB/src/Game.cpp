/*
 * Game.cpp
 *
 *  Created on: Aug 1, 2018
 *      Author: Samuel Tan
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"

int run()
{
  Window window;

  while (!window.shouldClose())
  {
    window.update();
    window.render();

    glfwPollEvents();
  }

  return 0;
}
