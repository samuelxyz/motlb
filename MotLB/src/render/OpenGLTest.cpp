/*
 * OpenGLTest.cpp
 *
 *  Created on: Jul 26, 2018
 *      Author: Samuel Tan
 *
 *      TODO const correctness
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>

#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"

#ifdef MOTLB_DEBUG
static void printGLFWError(int error, const char* description)
{
  fprintf(stderr, "[GLFW Error] %s\n", description);
}

void APIENTRY printGLDebug(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
//  std::cout << "[GL][" << __FILE__ << ": line " << __LINE__ << "] " << message << std::endl;
  std::cout << "[GL] " << message << std::endl;
}
#endif

void renderBasic()
{
  glBegin(GL_TRIANGLES);

  glVertex2f(-0.7f, -0.2f);
  glVertex2f(-0.7f, -0.7f);
  glVertex2f(-0.2f, -0.7f);

  glVertex2f(0.7f, 0.2f);
  glVertex2f(0.7f, 0.7f);
  glVertex2f(0.2f, 0.7f);

  glEnd();
}

void renderArray()
{
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderElements(int indices)
{
  glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
}

void changeColor(float& color, float& increment)
{
  color += increment;
  if (color >= 1.0f)
  {
    color = 1.0f;
    increment *= -1;
  }
  else if (color <= 0.0f)
  {
    color = 0.0f;
    increment *= -1;
  }
}

int openGLTest()
{
  GLFWwindow* window;

#ifdef MOTLB_DEBUG
  glfwSetErrorCallback(printGLFWError);
#endif

  /* Initialize the library */
  if (!glfwInit())
    return -1;

#ifdef MOTLB_DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 640, "MotLB OpenGL Test", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  /* Initialize GLEW */
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "GLEW is not ok" << std::endl;
    return -1;
  }

#ifdef MOTLB_DEBUG
  std::cout << "Running in debug mode with OpenGL version " <<
      glGetString(GL_VERSION) << std::endl;
  glDebugMessageCallback(printGLDebug, nullptr);
#endif

  float positions[] = {

      -0.7f, -0.2f, // lower left
      -0.7f, -0.7f,
      -0.2f, -0.7f,

      -0.7f,  0.2f, // upper left
      -0.7f,  0.7f,
      -0.2f,  0.7f,

       0.7f,  0.2f, // upper right
       0.7f,  0.7f,
       0.2f,  0.7f,

       0.7f, -0.2f, // lower right
       0.7f, -0.7f,
       0.2f, -0.7f,

      -0.2f, -0.2f, // center square
      -0.2f,  0.2f,
       0.2f,  0.2f,
       0.2f, -0.2f
  };

  GLuint indices[] = {

      0, 1, 2,

      3, 4, 5,

      6, 7, 8,

      9, 10, 11,

      12, 13, 14,
      12, 14, 15
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  render::VertexBuffer vbo((const void*)positions, sizeof(positions), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);

  render::IndexBuffer ibo(indices, sizeof(indices)/sizeof(GLuint), GL_STATIC_DRAW);

  render::ShaderProgram shaders("resources/shaders/Basic.shader");

  float red = 0.0f, redIncrement = 0.02f,
      green = 0.0f, greenIncrement = 0.01f,
      blue = 0.0f, blueIncrement = 0.005f;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    shaders.bind();
    changeColor(red, redIncrement);
    changeColor(green, greenIncrement);
    changeColor(blue, blueIncrement);
    shaders.setUniform4f("u_Color", red, green, blue, 1.0f);

    glBindVertexArray(vao);

    ibo.bind();

//    renderArray();
    renderElements(ibo.getNumIndices());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
