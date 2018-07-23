/*
 * Box.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include "Box.h"

#include <cmath>


Box::Box(Vec2 position, double xMin, double xMax,
    double yMin, double yMax, double angle)
  : position(position), xMin(xMin), xMax(xMax),
    yMin(yMin), yMax(yMax), angle(angle)
{
}

Box::Box(Vec2 corner1, Vec2 corner2)
  : position(0.5*(corner1 + corner2)),
    xMin(std::min(corner1.getX(), corner2.getX())),
    xMax(std::max(corner1.getX(), corner2.getX())),
    yMin(std::min(corner1.getY(), corner2.getY())),
    yMax(std::max(corner1.getY(), corner2.getY())),
    angle(0)
{
}

Vec2 Box::toRelative(Vec2 point) const
{
  point += position;
  point.rotateBy(-angle);
  return point;
}

Vec2 Box::toAbs(Vec2 point) const
{
  point.rotateBy(angle);
  point += position;
  return point;
}

bool Box::containsAbs(Vec2 point) const
{
  return containsRel(toRelative(point));
}

void Box::absCorners(std::array<Vec2, 4>& toFill) const
{
  relCorners(toFill);
  for (Vec2 c : toFill)
  {
    c = toAbs(c);
  }
}

Vec2 Box::toClosestEdge(Vec2 point) const
{
  using namespace std;

  double minDistX, minDistY;
  point = toRelative(point);

  // which X side is closer?
  minDistX = ( abs(xMax - point.getX()) < abs(xMin - point.getX()) )
    ? xMax - point.getX() : xMin - point.getX(); // signs will reflect direction

  // which Y side is closer?
  minDistY = ( abs(yMax - point.getY()) < abs(yMin - point.getY()) )
    ? yMax - point.getY() : yMin - point.getY();

  // is X or Y closer?
  return (abs(minDistX) > abs(minDistY))?
      Vec2(0, minDistY) : Vec2(minDistX, 0);
}

bool Box::overlaps(const Box& b1, const Box& b2)
{
  return (b1.overlapsOneWay(b2)
       || b2.overlapsOneWay(b1));
}

Vec2 Box::collide(const Box& standOn, const Box& giveWay)
{
  Vec2 v1 = standOn.collideOneWay(giveWay);
  Vec2 v2 = -giveWay.collideOneWay(standOn);

  if (v1.isZero())
    return v2;
  if (v2.isZero())
    return v1;

  return std::min(v1, v2);
}

bool Box::containsRel(Vec2 point) const
{
  return xMin <= point.getX() && point.getX() <= xMax &&
      yMin <= point.getY() && point.getY() <= yMax;
}

/*static*/ void Box::relCorners(std::array<Vec2, 4>& toFill) const
{
  toFill = {
    Vec2(xMax, yMax),
    Vec2(xMin, yMax),
    Vec2(xMin, yMin),
    Vec2(xMax, yMin)
  };
}

bool Box::overlapsOneWay(const Box& other) const
{
  std::array<Vec2, 4> corners;
  other.absCorners(corners);

  for (Vec2 v : corners)
  {
    if (containsAbs(v))
      return true;
  }
  return false;
}

double Box::getWidth() const
{
  return xMax - xMin;
}

double Box::getHeight() const
{
  return yMax - yMin;
}

Vec2 Box::collideOneWay(const Box& other) const
{
  std::array<Vec2, 4> corners;
  other.absCorners(corners);
  std::array<Vec2, 4> inCorners;
  int numInCorners = 0;

  for (int i = 0; i < 4; i++)
  {
    if (containsAbs(corners[i]))
    {
      inCorners[numInCorners] = corners[i];
      numInCorners++;
    }
  }

  if (!numInCorners)
    return Vec2(); // no translation needed

  // the plan is to try moving other in each
  // cardinal direction, find shortest move
}

