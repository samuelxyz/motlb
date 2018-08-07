/*
 * Main.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include <iostream>

#include "geometry/Box.h"
#include "geometry/Vec2.h"
#include "Values.h"

//int uniformShaderTest();
//int vertexColorShaderTest();
//int coloredBoxBatchTest();
int battleRenderTest();

geometry::Vec2 getVector()
{
  using namespace std;

  cout << "Input vector:" << endl;
  double x, y;
  cin >> x >> y;
  geometry::Vec2 v(x, y);
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
  geometry::Vec2 v1 = getVector();
  v1.rotateBy(getDouble());
  std::cout << v1;
}

geometry::Box getBox()
{
  using namespace std;

  cout << "Input box: Pos, angle, xMin, xMax, yMin, yMax" << endl;
  geometry::Vec2 pos(getVector());
  double angle, xMin, xMax, yMin, yMax;
  cin >> angle >> xMin >> xMax >> yMin >> yMax;
  cout << "Box received" << endl;

  return geometry::Box(pos, angle, xMin, xMax, yMin, yMax);
}

void boxTest()
{
  //  Box b1(getBox()), b2(getBox());
  geometry::Box b1(geometry::Vec2(), 3.14159/2, -1, 1, -1, 1);
  geometry::Box b2(geometry::Vec2(1, 1), 0, -0.3, 0.3, -0.3, 0.3);
  std::cout << geometry::Box::collide(b1, b2);
}

int main()
{
  //  std::srand(static_cast<unsigned int>(std::time(0)));
  //  boxTest();

  Values::initRand();
  return battleRenderTest();
}
