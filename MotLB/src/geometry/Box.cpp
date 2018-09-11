/*
 * Box.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#include <Box.h>
#include <Values.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
//#include <iostream>


namespace geometry
{
  Box::Box()
  : Box(geometry::Vec2(), 0, 0, 0, 0, 0)
  {
  }

  Box::Box(Vec2 position, double angle,
      double xMin, double xMax,
      double yMin, double yMax)
  : position(position), angle(angle),
    xMin(xMin), xMax(xMax),
    yMin(yMin), yMax(yMax)
  {
//    std::cout << "Initialized: " << *this << std::endl;
  }

  Box::Box(Vec2 corner1, Vec2 corner2)
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

  Vec2 Box::toRelative(Vec2 point) const
  {
    point -= position;
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

    for (Vec2& c : toFill)
    {
      c = toAbs(c);
    }
  }

  Vec2 Box::randomInteriorPos() const
  {
    double x = Values::random(xMin, xMax);
    double y = Values::random(yMin, yMax);
    return toAbs({ x, y });
  }

  Vec2 Box::toClosestEdge(Vec2 point) const
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
        Vec2(0, minDistY) : Vec2(minDistX, 0);
  }

  double Box::toIntersection(Vec2 pos, Vec2 dir) const
  {
    // to relative
    pos = toRelative(pos);
    dir.rotateBy(-angle);

    // how far dir should be traced to hit different lines
    std::vector<double> dirScalars;

    // line intersections
    if (dir.x)
    {
      dirScalars.emplace_back((xMin - pos.x)/dir.x);
      dirScalars.emplace_back((xMax - pos.x)/dir.x);
    }
    if (dir.y)
    {
      dirScalars.emplace_back((yMax - pos.y)/dir.y);
      dirScalars.emplace_back((yMin - pos.y)/dir.y);
    }

    Box edgeTest = Box(*this, 1.0001); // in case of rounding errors
    for (unsigned int i = 0; i < dirScalars.size();)
    {
      // backward or not an edge of the box
      if (dirScalars[i] <= 0 || !edgeTest.containsRel(pos + dir*dirScalars[i]) )
        dirScalars.erase(dirScalars.begin() + i);
      else
        ++i;
    }

    if (!dirScalars.empty())
      return *std::min_element(dirScalars.begin(), dirScalars.end());
    else
      return -1.0; // no intersection
  }

  bool Box::containsRel(Vec2 point) const
  {
    return xMin <= point.x && point.x <= xMax &&
        yMin <= point.y && point.y <= yMax;
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

  /*static*/ Vec2 Box::collide(const Box& standOn, const Box& giveWay)
  {
    Vec2 v1 = standOn.collideOneWay(giveWay);
    Vec2 v2 = -giveWay.collideOneWay(standOn);

    if (v1.isZero() || v2.isZero())
      return Vec2();

    return std::min(v1, v2);
  }

  // abs result using SAT theorem
  Vec2 Box::collideOneWay(const Box& other) const
  {
    // project all corners of other box
    std::array<Vec2, 4> corners;
    other.absCorners(corners);
    for (Vec2& v : corners)
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

    Vec2 xMove(
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

    Vec2 yMove(
        0,
        collide1D({yMin, yMax}, {otherYMin, otherYMax})
    );

    if (yMove.isZero())
      return Vec2();

    Vec2 shorterMove( std::min(xMove, yMove) );
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

  Vec2 Box::contain(const Box& target) const
  {
    Vec2 res;

    std::array<Vec2, 4> corners;
    target.absCorners(corners);

    for (Vec2& c : corners)
    {
      c = toRelative(c);

      // x
      if (c.x > xMax)
      {
        if (res.x > 0)
          return Vec2(); // can't fit
        else if (xMax - c.x < res.x)
          res.x = xMax - c.x;
      }
      else if (c.x < xMin)
      {
        if (res.x < 0)
          return Vec2(); // can't fit
        else if (xMin - c.x > res.x)
          res.x = xMin - c.x;
      }

      // y
      if (c.y > yMax)
      {
        if (res.y > 0)
          return Vec2(); // can't fit
        if (yMax - c.y < res.y)
          res.y = yMax - c.y;
      }
      else if (c.y < yMin)
      {
        if (res.y < 0)
          return Vec2(); // can't fit
        if (yMin - c.y > res.y)
          res.y = yMin - c.y;
      }
    }

    res.rotateBy(angle);
    return res;
  }
}

