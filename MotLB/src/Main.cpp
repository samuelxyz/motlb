/*
 * Main.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>

#include "Box.h"
#include "Vec2.h"

Vec2 getVector()
{
  using namespace std;

  cout << "Input vector:" << endl;
  double x, y;
  cin >> x >> y;
  Vec2 v(x, y);
  cout << "Vector received: " << v << endl;
  return v;
}

double getDouble()
{
  using namespace std;

  cout << "Input double:" << endl;
  double d;
  cin >> d;
  return d;
}

void vectorTest()
{
  Vec2 v1 = getVector();
  v1.rotateBy(getDouble());
  std::cout << v1;
}

Box getBox()
{
  using namespace std;

  cout << "Input box: Pos, angle, xMin, xMax, yMin, yMax" << endl;
  Vec2 pos(getVector());
  double angle, xMin, xMax, yMin, yMax;
  cin >> angle >> xMin >> xMax >> yMin >> yMax;
  cout << "Box received" << endl;

  return Box(pos, angle, xMin, xMax, yMin, yMax);
}

void boxTest()
{
  //  Box b1(getBox()), b2(getBox());
  Box b1(Vec2(), 3.14159/2, -1, 1, -1, 1);
  Box b2(Vec2(1, 1), 0, -0.3, 0.3, -0.3, 0.3);
  std::cout << Box::collide(b1, b2);
}

static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
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

int glfwTest()
{
  GLFWwindow* window;

  glfwSetErrorCallback(error_callback);

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Initialize GLEW */
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "GLEW is not ok" << std::endl;
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {

      -0.7f, -0.2f,
      -0.7f, -0.7f,
      -0.2f, -0.7f,

       0.7f,  0.2f,
       0.7f,  0.7f,
       0.2f,  0.7f,
  };

  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    renderArray();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

int main()
{
  //  std::srand(static_cast<unsigned int>(std::time(0)));
  //  boxTest();

  return glfwTest();
}
