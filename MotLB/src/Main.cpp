/*
 * Main.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include <iostream>

#include "Vec2.h"

Vec2 getVector()
{
  using namespace std;
  cout << "Input vector:" << endl;
  int x, y;
  cin >> x >> y;
  return Vec2(x, y);
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

int main()
{
  vectorTest();
  return 0;
}
