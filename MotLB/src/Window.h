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

class Window
{
  private:
    GLFWwindow* window;

  public:
    Window();
    Window(int width, int height, const char* title, GLFWmonitor* monitor);
    ~Window();

    bool shouldClose() const;
    void swapBuffers() const;

    static void printGLFWError(int error, const char* description);
    static void APIENTRY printGLDebug(GLenum source, GLenum type, GLuint id,
          GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    GLFWwindow* getGLFWwindow() const { return window; }
};

#endif /* WINDOW_H_ */
