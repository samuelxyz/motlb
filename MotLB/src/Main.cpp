/*
 * Main.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include <iostream>

#include "Box.h"
#include "Vec2.h"

int openGLTest();

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

int main()
{
  //  std::srand(static_cast<unsigned int>(std::time(0)));
  //  boxTest();

  return openGLTest();
}
