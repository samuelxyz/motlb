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
#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

struct ShaderSources
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSources parseShader(const std::string& filepath)
{
  std::ifstream stream(filepath);
  std::stringstream streams[2];
  std::string line;
  enum class ShaderType {
      NONE = -1, VERTEX, FRAGMENT
  } type(ShaderType::NONE);

  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }
    else
    {
      if (type != ShaderType::NONE)
        streams[(int)type] << line << '\n';
    }
  }

  return {streams[0].str(), streams[1].str()};
}

static GLuint compileShader(GLenum type, std::string& source)
{
  GLuint shader = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  // error handling
  int compileSuccess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
  if (compileSuccess == GL_FALSE)
  {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    char* msg = new char[length];
    glGetShaderInfoLog(shader, length, &length, msg);
    std::cerr << "Failed to compile " << ((type == GL_VERTEX_SHADER) ?
        "vertex" : "fragment") << " shader!" << std::endl;
    std::cerr << msg << std::endl;

    delete msg;

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

static GLuint createShaders(std::string& vertexShader, std::string& fragmentShader)
{
  GLuint program = glCreateProgram();
  GLuint vertex   = compileShader(GL_VERTEX_SHADER, vertexShader);
  GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  assert(vertex != 0 && fragment != 0);

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return program;
}

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

  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);

  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  ShaderSources sources = parseShader("resources/shaders/Basic.shader");

  GLuint shaders = createShaders(sources.vertexSource, sources.fragmentSource);
  glUseProgram(shaders);

  int u_Color_loc = glGetUniformLocation(shaders, "u_Color");

  glDeleteProgram(shaders);

  float red = 0.0f, redIncrement = 0.05f,
      green = 0.0f, greenIncrement = 0.025f,
      blue = 0.0f, blueIncrement = 0.0125f;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    changeColor(red, redIncrement);
    changeColor(green, greenIncrement);
    changeColor(blue, blueIncrement);

    glUniform4f(u_Color_loc, red, green, blue, 1.0f);

//    renderArray();
    renderElements(sizeof(indices)/sizeof(unsigned int));

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
