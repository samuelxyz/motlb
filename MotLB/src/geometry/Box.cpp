/*
 * Box.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include "Box.h"

#include <cmath>
#include <iostream>

namespace geometry
{

  Box::Box(geometry::Vec2 position, double angle,
      double xMin, double xMax,
      double yMin, double yMax)
  : position(position), angle(angle),
    xMin(xMin), xMax(xMax),
    yMin(yMin), yMax(yMax)
  {
    std::cout << "Initialized: " << *this << std::endl;
  }

  Box::Box(geometry::Vec2 corner1, geometry::Vec2 corner2)
  : position(0.5*(corner1 + corner2)), angle(0),
    xMin(std::min(corner1.getX(), corner2.getX()) - position.getX()),
    xMax(std::max(corner1.getX(), corner2.getX()) - position.getX()),
    yMin(std::min(corner1.getY(), corner2.getY()) - position.getY()),
    yMax(std::max(corner1.getY(), corner2.getY()) - position.getY())
  {
  }

  std::ostream& operator<<(std::ostream& out, const Box& b)
  {
    using namespace std;
    out << "Box(position " << b.position << ", angle " <<
        b.angle << ", " << b.xMin << ", " << b.xMax <<
        ", " << b.yMin << ", " << b.yMax << ")";
    return out;
  }

  geometry::Vec2 Box::toRelative(geometry::Vec2 point) const
  {
    point -= position;
    point.rotateBy(-angle);
    return point;
  }

  geometry::Vec2 Box::toAbs(geometry::Vec2 point) const
  {
    point.rotateBy(angle);
    point += position;
    return point;
  }

  bool Box::containsAbs(geometry::Vec2 point) const
  {
    return containsRel(toRelative(point));
  }

  void Box::absCorners(std::array<geometry::Vec2, 4>& toFill) const
  {
    relCorners(toFill);

    // printing for debug
    //  std::cout << "relCorners:";
    //  for (geometry::Vec2 v : toFill)
    //    std::cout << " " << v;
    //  std::cout << std::endl;

    for (geometry::Vec2& c : toFill)
    {
      c = toAbs(c);
    }
  }

  geometry::Vec2 Box::toClosestEdge(geometry::Vec2 point) const
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
        geometry::Vec2(0, minDistY) : geometry::Vec2(minDistX, 0);
  }

  bool Box::overlaps(const Box& b1, const Box& b2)
  {
    return (b1.overlapsOneWay(b2)
        || b2.overlapsOneWay(b1));
  }

  geometry::Vec2 Box::collide(const Box& standOn, const Box& giveWay)
  {
    geometry::Vec2 v1 = standOn.collideOneWay(giveWay);
    geometry::Vec2 v2 = -giveWay.collideOneWay(standOn);

    //  std::cout << "Collision results: " << v1 << " " << v2 << std::endl;

    if (v1.isZero())
      return v2;
    if (v2.isZero())
      return v1;

    return std::min(v1, v2);
  }

  bool Box::containsRel(geometry::Vec2 point) const
  {
    return xMin <= point.getX() && point.getX() <= xMax &&
        yMin <= point.getY() && point.getY() <= yMax;
  }

  /*static*/ void Box::relCorners(std::array<geometry::Vec2, 4>& toFill) const
  {
    toFill = {
        geometry::Vec2(xMax, yMax),
        geometry::Vec2(xMin, yMax),
        geometry::Vec2(xMin, yMin),
        geometry::Vec2(xMax, yMin)
    };
  }

  bool Box::overlapsOneWay(const Box& other) const
  {
    std::array<geometry::Vec2, 4> corners;
    other.absCorners(corners);

    for (geometry::Vec2& v : corners)
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

  geometry::Vec2 Box::collideOneWay(const Box& other) const
  {
    std::array<geometry::Vec2, 4> corners;
    other.absCorners(corners);

    //  std::cout << "absCorners:"; // debug printing
    //  for (geometry::Vec2& v : corners)
    //    std::cout << " " << v;
    //  std::cout << std::endl;

    std::vector<geometry::Vec2> inCorners;
    for (geometry::Vec2 v : corners)
      if (containsAbs(v))
        inCorners.push_back(v);

    //  std::cout << "inCorners:"; // debug printing
    //  for (geometry::Vec2& v : inCorners)
    //    std::cout << " " << v;
    //  std::cout << std::endl << *this << std::endl << std::endl;

    if (inCorners.empty())
    {
      //    std::cout << "inCorners empty" << std::endl;
      return geometry::Vec2(); // no translation needed
    }

    // the plan is to try moving other in each
    // cardinal direction, find shortest move
    geometry::Vec2 candidateShortest;
    geometry::Vec2 finalShortest;

    // consider movement +y
    finalShortest.setRect(0, yMax - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(0, -1)).getY());

    // -y
    candidateShortest.setRect(0, yMin - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(0, 1)).getY());
    if (candidateShortest < finalShortest)
      finalShortest = candidateShortest;

    // +x
    candidateShortest.setRect(xMax - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(-1, 0)).getX(), 0);
    if (candidateShortest < finalShortest)
      finalShortest = candidateShortest;

    // -x
    candidateShortest.setRect(xMin - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(1, 0)).getX(), 0);
    if (candidateShortest < finalShortest)
      finalShortest = candidateShortest;

    return finalShortest;
  }

  geometry::Vec2 Box::contain(const Box& target) const
  {
    geometry::Vec2 res;

    std::array<geometry::Vec2, 4> corners;
    target.absCorners(corners);

    for (geometry::Vec2& c : corners)
    {
      c = toRelative(c);

      // x
      if (c.getX() > xMax)
      {
        if (res.getX() > 0)
          return geometry::Vec2(); // can't fit
        else if (xMax - c.getX() < res.getX())
          res.setRect(xMax - c.getX(), res.getY());
      }
      else if (c.getX() < xMin)
      {
        if (res.getX() < 0)
          return geometry::Vec2(); // can't fit
        else if (xMin - c.getX() > res.getX())
          res.setRect(xMin - c.getX(), res.getY());
      }

      // y
      if (c.getY() > yMax)
      {
        if (res.getY() > 0)
          return geometry::Vec2(); // can't fit
        if (yMax - c.getY() < res.getY())
          res.setRect(res.getX(), yMax - c.getY());
      }
      else if (c.getY() < yMin)
      {
        if (res.getY() < 0)
          return geometry::Vec2(); // can't fit
        if (yMin - c.getY() > res.getY())
          res.setRect(res.getX(), yMin - c.getY());
      }
    }

//    return toAbs(res);
    res.rotateBy(angle);
    return res;
  }
}
