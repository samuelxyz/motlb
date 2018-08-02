/*
 * OpenGLTest.cpp
 *
 *  Created on: Jul 26, 2018
 *      Author: Samuel Tan
 *
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../entity/Entity.h"
#include "../geometry/Box.h"
#include "../geometry/Vec2.h"
#include "../graphics/ColoredBoxBatch.h"
#include "../graphics/IndexBuffer.h"
#include "../graphics/Renderer.h"
#include "../graphics/ShaderProgram.h"
#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../Window.h"

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

int uniformShaderTest()
{
  Window window;

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

  graphics::ShaderProgram shaderProgram("resources/shaders/UniformColorShader.glsl");

  graphics::VertexArray vertexArray;
  vertexArray.addAttribute("position", GL_FLOAT, 2);

  graphics::VertexBuffer vertexBuffer((const void*)positions, sizeof(positions), GL_STATIC_DRAW);
  vertexArray.applyAttributesWithBuffer(vertexBuffer, shaderProgram);

  graphics::IndexBuffer indexBuffer(indices, sizeof(indices)/sizeof(GLuint), GL_STATIC_DRAW);

  float red = 0.0f, redIncrement = 0.02f,
      green = 0.0f, greenIncrement = 0.01f,
      blue = 0.0f, blueIncrement = 0.005f;

  /* Loop until the user closes the window */
  while (!window.shouldClose())
  {
    /* Render here */
    graphics::Renderer::clearScreen();

    changeColor(red, redIncrement);
    changeColor(green, greenIncrement);
    changeColor(blue, blueIncrement);
    shaderProgram.bind();
    shaderProgram.setUniform4f("u_Color", red, green, blue, 1.0f);

    graphics::Renderer::draw(GL_TRIANGLES, vertexArray, indexBuffer, shaderProgram);

    /* Swap front and back buffers */
    window.swapBuffers();

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

int vertexColorShaderTest()
{
  Window window;

  float vertices[] = {

//   |  x      y   |   r     g     b     a  |
      -0.7f, -0.2f,   1.0f, 0.0f, 0.0f, 1.0f, // lower left
      -0.7f, -0.7f,   1.0f, 0.0f, 0.0f, 1.0f,
      -0.2f, -0.7f,   1.0f, 0.0f, 0.0f, 1.0f,

      -0.7f,  0.2f,   0.0f, 1.0f, 0.0f, 1.0f, // upper left
      -0.7f,  0.7f,   0.0f, 1.0f, 0.0f, 1.0f,
      -0.2f,  0.7f,   0.0f, 1.0f, 0.0f, 1.0f,

       0.7f,  0.2f,   0.0f, 0.0f, 1.0f, 1.0f, // upper right
       0.7f,  0.7f,   0.0f, 0.0f, 1.0f, 1.0f,
       0.2f,  0.7f,   0.0f, 0.0f, 1.0f, 1.0f,

       0.7f, -0.2f,   1.0f, 1.0f, 0.0f, 1.0f, // lower right
       0.7f, -0.7f,   1.0f, 1.0f, 0.0f, 1.0f,
       0.2f, -0.7f,   1.0f, 1.0f, 0.0f, 1.0f,

      -0.2f, -0.2f,   1.0f, 0.0f, 0.0f, 1.0f, // center square
      -0.2f,  0.2f,   0.0f, 1.0f, 0.0f, 1.0f,
       0.2f,  0.2f,   0.0f, 0.0f, 1.0f, 1.0f,
       0.2f, -0.2f,   1.0f, 1.0f, 0.0f, 1.0f
  };

  GLuint indices[] = {

      0, 1, 2,

      3, 4, 5,

      6, 7, 8,

      9, 10, 11,

      12, 13, 14,
      12, 14, 15
  };

  graphics::ShaderProgram shaderProgram("resources/shaders/VertexColorShader.glsl");

  graphics::VertexArray vertexArray;
  vertexArray.addAttribute("position", GL_FLOAT, 2);
  vertexArray.addAttribute("color", GL_FLOAT, 4);

  graphics::VertexBuffer vertexBuffer((const void*)vertices, sizeof(vertices), GL_STATIC_DRAW);
  vertexArray.applyAttributesWithBuffer(vertexBuffer, shaderProgram);

  graphics::IndexBuffer indexBuffer(indices, sizeof(indices)/sizeof(GLuint), GL_STATIC_DRAW);

  /* Loop until the user closes the window */
  while (!window.shouldClose())
  {
    /* Render here */
    graphics::Renderer::clearScreen();
    graphics::Renderer::draw(GL_TRIANGLES, vertexArray, indexBuffer, shaderProgram);

    /* Swap front and back buffers */
    window.swapBuffers();

    /* Poll for and process events */
    glfwPollEvents();
  }

  return 0;
}

int coloredBoxBatchTest()
{
  Window window;

  graphics::ShaderProgram shaderProgram("resources/shaders/ColoredBoxBatch.glsl");
  graphics::ColoredBoxBatch cbb(1, shaderProgram);

  geometry::Box box(geometry::Vec2(-0.5, -0.5), geometry::Vec2(0.5, 0.5));
  Values::Color blue(entity::Entity::getTeamColor(entity::Entity::Team::BLUE));
  cbb.add({blue, box});

  /* Loop until the user closes the window */
  while (!window.shouldClose())
  {
    /* Render here */
    graphics::Renderer::clearScreen();
    cbb.renderAll();

    /* Swap front and back buffers */
    window.swapBuffers();

    /* Poll for and process events */
    glfwPollEvents();
  }

  return 0;
}

