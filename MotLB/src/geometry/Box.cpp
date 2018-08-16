/*
 * Box.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include <Box.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
//#include <iostream>

#include "../Values.h"

namespace geometry
{
  Box::Box(geometry::Vec2 position, double angle,
      double xMin, double xMax,
      double yMin, double yMax)
  : position(position), angle(angle),
    xMin(xMin), xMax(xMax),
    yMin(yMin), yMax(yMax)
  {
//    std::cout << "Initialized: " << *this << std::endl;
  }

  Box::Box(geometry::Vec2 corner1, geometry::Vec2 corner2)
  : position(0.5*(corner1 + corner2)), angle(0),
    xMin(std::min(corner1.x, corner2.x) - position.x),
    xMax(std::max(corner1.x, corner2.x) - position.x),
    yMin(std::min(corner1.y, corner2.y) - position.y),
    yMax(std::max(corner1.y, corner2.y) - position.y)
  {
  }

  Box::Box(const Box& source, double scale /*= 1.0*/)
  : position(source.position), angle(source.angle),
    xMin(source.xMin * scale),
    xMax(source.xMax * scale),
    yMin(source.yMin * scale),
    yMax(source.yMax * scale)
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

    for (geometry::Vec2& c : toFill)
    {
      c = toAbs(c);
    }
  }

  geometry::Vec2 Box::randomInteriorPos() const
  {
    double x = Values::random(xMin, xMax);
    double y = Values::random(yMin, yMax);
    return toAbs({ x, y });
  }

  geometry::Vec2 Box::toClosestEdge(geometry::Vec2 point) const
  {
    using namespace std;

    double minDistX, minDistY;
    point = toRelative(point);

    // which X side is closer?
    minDistX = ( abs(xMax - point.x) < abs(xMin - point.x) )
        ? xMax - point.x : xMin - point.x; // signs will reflect direction

    // which Y side is closer?
    minDistY = ( abs(yMax - point.y) < abs(yMin - point.y) )
        ? yMax - point.y : yMin - point.y;

    // is X or Y closer?
    return (abs(minDistX) > abs(minDistY))?
        geometry::Vec2(0, minDistY) : geometry::Vec2(minDistX, 0);
  }

//  bool Box::overlaps(const Box& b1, const Box& b2)
//  {
//    return (b1.overlapsOneWay(b2)
//        || b2.overlapsOneWay(b1));
//  }

//  geometry::Vec2 Box::collide(const Box& standOn, const Box& giveWay)
//  {
//    geometry::Vec2 v1 = standOn.collideOneWay(giveWay);
//    geometry::Vec2 v2 = -giveWay.collideOneWay(standOn);
//
//    //  std::cout << "Collision results: " << v1 << " " << v2 << std::endl;
//
//    if (v1.isZero())
//      return v2;
//    if (v2.isZero())
//      return v1;
//
//    return std::min(v1, v2);
//  }

  bool Box::containsRel(geometry::Vec2 point) const
  {
    return xMin <= point.x && point.x <= xMax &&
        yMin <= point.y && point.y <= yMax;
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

//  bool Box::overlapsOneWay(const Box& other) const
//  {
//    std::array<geometry::Vec2, 4> corners;
//    other.absCorners(corners);
//
//    for (geometry::Vec2& v : corners)
//    {
//      if (containsAbs(v))
//        return true;
//    }
//    return false;
//  }

  double Box::getWidth() const
  {
    return xMax - xMin;
  }

  double Box::getHeight() const
  {
    return yMax - yMin;
  }

  double Box::getLongestRadius() const
  {
    double longerX = std::max(abs(xMax), abs(xMin));
    double longerY = std::max(abs(yMax), abs(yMin));

    return hypot(longerX, longerY);
  }

  geometry::Vec2 geometry::Box::collide(const Box& standOn, const Box& giveWay)
  {
    geometry::Vec2 v1 = standOn.collideOneWay(giveWay);
    geometry::Vec2 v2 = -giveWay.collideOneWay(standOn);

    if (v1.isZero() || v2.isZero())
      return Vec2();

    return std::min(v1, v2);
  }

  // abs result using SAT theorem
  geometry::Vec2 geometry::Box::collideOneWay(const Box& other) const
  {
    // project all corners of other box
    std::array<geometry::Vec2, 4> corners;
    other.absCorners(corners);
    for (geometry::Vec2& v : corners)
      v = toRelative(v);

    // test for overlap along this box's x axis
    double otherXMax = std::max(
        std::max(corners[0].x, corners[1].x),
        std::max(corners[2].x, corners[3].x)
    );

    double otherXMin = std::min(
        std::min(corners[0].x, corners[1].x),
        std::min(corners[2].x, corners[3].x)
    );

    geometry::Vec2 xMove(
        collide1D({xMin, xMax}, {otherXMin, otherXMax}),
        0
    );

    // short circuit shortcut
    if (xMove.isZero())
      return Vec2();

    // now test along y axis
    double otherYMax = std::max(
            std::max(corners[0].y, corners[1].y),
            std::max(corners[2].y, corners[3].y)
        );

    double otherYMin = std::min(
        std::min(corners[0].y, corners[1].y),
        std::min(corners[2].y, corners[3].y)
    );

    geometry::Vec2 yMove(
        0,
        collide1D({yMin, yMax}, {otherYMin, otherYMax})
    );

    if (yMove.isZero())
      return Vec2();

    geometry::Vec2 shorterMove( std::min(xMove, yMove) );
    shorterMove.rotateBy(angle);
    return shorterMove;
  }

  double Box::collide1D(const Box::Interval standOn, const Box::Interval giveWay)
  {
    // no overlap
    if (giveWay.max <= standOn.min ||
        giveWay.min >= standOn.max)
      return 0.0;

    // now we know there's definitely an overlap
    // move giveWay to the right or left

    double rightMove = standOn.max - giveWay.min;
    double leftMove = standOn.min - giveWay.max;

    return (abs(rightMove) < abs(leftMove)) ? rightMove : leftMove;
  }

//  geometry::Vec2 Box::collideOneWay(const Box& other) const
//  {
//    std::array<geometry::Vec2, 4> corners;
//    other.absCorners(corners);
//
//    std::vector<geometry::Vec2> inCorners;
//    for (geometry::Vec2& c : corners)
//    {
//      geometry::Vec2 v = toRelative(c);
//      if (containsRel(v))
//        inCorners.push_back(v);
//    }
//
//    if (inCorners.empty())
//    {
//      //    std::cout << "inCorners empty" << std::endl;
//      return geometry::Vec2(); // no translation needed
//    }
//
//    // the plan is to try moving other in each
//    // cardinal direction, find shortest move
//    geometry::Vec2 candidateShortest;
//    geometry::Vec2 finalShortest;
//
//    // consider movement +y
//    finalShortest.setRect(0, yMax - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(0, -1)).y);
//
//    // -y
//    candidateShortest.setRect(0, yMin - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(0, 1)).y);
//    if (candidateShortest < finalShortest)
//      finalShortest = candidateShortest;
//
//    // +x
//    candidateShortest.setRect(xMax - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(-1, 0)).x, 0);
//    if (candidateShortest < finalShortest)
//      finalShortest = candidateShortest;
//
//    // -x
//    candidateShortest.setRect(xMin - geometry::Vec2::mostExtreme(inCorners, geometry::Vec2(1, 0)).x, 0);
//    if (candidateShortest < finalShortest)
//      finalShortest = candidateShortest;
//
//    finalShortest.rotateBy(angle); // new
//    return finalShortest;
//  }

  geometry::Vec2 Box::contain(const Box& target) const
  {
    geometry::Vec2 res;

    std::array<geometry::Vec2, 4> corners;
    target.absCorners(corners);

    for (geometry::Vec2& c : corners)
    {
      c = toRelative(c);

      // x
      if (c.x > xMax)
      {
        if (res.x > 0)
          return geometry::Vec2(); // can't fit
        else if (xMax - c.x < res.x)
          res.x = xMax - c.x;
      }
      else if (c.x < xMin)
      {
        if (res.x < 0)
          return geometry::Vec2(); // can't fit
        else if (xMin - c.x > res.x)
          res.x = xMin - c.x;
      }

      // y
      if (c.y > yMax)
      {
        if (res.y > 0)
          return geometry::Vec2(); // can't fit
        if (yMax - c.y < res.y)
          res.y = yMax - c.y;
      }
      else if (c.y < yMin)
      {
        if (res.y < 0)
          return geometry::Vec2(); // can't fit
        if (yMin - c.y > res.y)
          res.y = yMin - c.y;
      }
    }

//    return toAbs(res);
    res.rotateBy(angle);
    return res;
  }
}

