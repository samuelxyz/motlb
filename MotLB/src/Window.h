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
#include <string>

class Battle;
namespace geometry { struct Vec2; }

class Window
{
  private:
    GLFWwindow* window;
    std::string title;
    Battle* battle;

  public:
    Window();
    Window(int width, int height, const char* title, GLFWmonitor* monitor);
    ~Window();

    bool shouldClose() const;
    void swapBuffers() const;

    void setBattle(Battle*);
    void setTitleMessage(const std::string& msg);

    static void printGLFWError(int error, const char* description);
    static void APIENTRY printGLDebug(GLenum source, GLenum type, GLuint id,
          GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    static void handleKey(GLFWwindow*, int key, int scancode, int action, int mods);
    static void handleMouseButton(GLFWwindow*, int button, int action, int mods);
    geometry::Vec2 getMousePos() const;

    GLFWwindow* getGLFWwindow() const { return window; }
};

#endif /* WINDOW_H_ */
