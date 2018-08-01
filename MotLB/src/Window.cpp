/*
 * Window.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#include "Window.h"
#include <cassert>
#include <iostream>

Window::Window() : Window(640, 640, "MotLB", nullptr)
{
}

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor)
{

#ifdef MOTLB_DEBUG /////////////////////////
  glfwSetErrorCallback(printGLFWError);
#endif /////////////////////////////////////

  /* Initialize the library */
  assert(glfwInit());

#ifdef MOTLB_DEBUG /////////////////////////
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif /////////////////////////////////////

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(width, height, title, monitor, NULL);
  if (!window)
  {
    glfwTerminate();
    assert(false);
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Set refresh rate to once per frame (60 Hz) */
  glfwSwapInterval(1);

  /* Initialize GLEW */
  assert(glewInit() == GLEW_OK);

#ifdef MOTLB_DEBUG /////////////////////////

  std::cout << "Running in debug mode with OpenGL version " <<
      glGetString(GL_VERSION) << std::endl;
  glDebugMessageCallback(printGLDebug, nullptr);

#endif /////////////////////////////////////
}

Window::~Window()
{
  glfwTerminate();
}

#ifdef MOTLB_DEBUG /////////////////////////

  void Window::printGLFWError(int error, const char* description)
  {
    fprintf(stderr, "[GLFW Error] %s\n", description);
  }

void Window::swapBuffers() const
{
  glfwSwapBuffers(window);
}

  void APIENTRY Window::printGLDebug(GLenum source, GLenum type, GLuint id,
      GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
  {
    //  std::cout << "[GL][" << __FILE__ << ": line " << __LINE__ << "] " << message << std::endl;
    std::cout << "[GL] " << message << std::endl;
}

#endif /////////////////////////////////////

bool Window::shouldClose() const
{
  return glfwWindowShouldClose(window);
}
