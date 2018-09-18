/*
 * Window.h
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Battle.h>
#include <GUIComponent.h>
#include <MouseHandler.h>
#include <Renderer.h>
#include <cassert>
#include <iostream>
#include <string>


class Battle;
namespace geometry { struct Vec2; }

class Window
{

#ifdef MOTLB_DEBUG //////////////////
  public:
    static void printGLFWError(int error, const char* description);
    static void __attribute__((__stdcall__)) printGLDebug(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif //////////////////////////////

  private:

    struct InitializerDummy // because initializer list order is bogus
    {                       // I need to create OpenGL context first
        GLFWwindow* window;

        InitializerDummy(int width, int height, const char* title, GLFWmonitor* monitor)
        {

#ifdef MOTLB_DEBUG
          glfwSetErrorCallback(printGLFWError);
#endif

          /* Initialize the library */
          assert(glfwInit());

#ifdef MOTLB_DEBUG
          glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
          glfwWindowHint(GLFW_SAMPLES, 4);
          glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
          glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

          /* Create a windowed mode window and its OpenGL context */
          window = glfwCreateWindow(width, height, title, monitor, NULL);
          if (!window)
          {
            glfwTerminate();
            assert(false && "GLFW window creation failed");
          }

          glfwSetWindowUserPointer(window, this);

          glfwSetKeyCallback(window, handleKey);
          glfwSetMouseButtonCallback(window, handleMouseButton);
          glfwSetScrollCallback(window, handleScroll);

          glfwSetCursor(window, glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));

          glfwSetWindowPos(window, 50, 50);
          glfwShowWindow(window);

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

          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

          glEnable(GL_POLYGON_SMOOTH);

          glEnable(GL_LINE_SMOOTH);
          glLineWidth(1);

          glEnable(GL_MULTISAMPLE);

        }
    } initializerDummy;

    GLFWwindow* window;
    std::string title;

    graphics::Renderer renderer;
    MouseHandler mouseHandler;
    gui::GUIComponent sidePanel, topPanel;
    Battle battle;

  public:
    Window();
    Window(int width, int height, const char* title, GLFWmonitor* monitor);
    ~Window();

    bool shouldClose() const;
    void update();
    void render();
    void refreshGUI();

    void setTitleMessage(const std::string& msg);

    static void handleKey(GLFWwindow*, int key, int scancode, int action, int mods);
    static void handleMouseButton(GLFWwindow*, int button, int action, int mods);
    static void handleScroll(GLFWwindow*, double xoffset, double yoffset);
    geometry::Vec2 getMousePos() const;
};

#endif /* WINDOW_H_ */
