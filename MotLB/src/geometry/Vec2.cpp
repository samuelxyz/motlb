/*
 * Vec2.cpp
 *
 *  Created on: Jul 12, 2018
 *      Author: xinyi
 */

#include "Vec2.h"

#include <cmath>
#include <iomanip>
#include <cassert>

namespace geometry
{

  // --- CONSTRUCTORS ---

  /*Vec2::Vec2(): x(0), y(0)
{}
Vec2::Vec2(double x, double y): x(x), y(y)
{}*/

  Vec2::Vec2(double x, double y) // both parameters default to 0
  {
    this->x = x;
    this->y = y;
  }

  // --- OPERATORS ---

  Vec2 operator+(const Vec2& v1, const Vec2& v2)
  {
    return Vec2(v1.getX() + v2.getX(), v1.getY() + v2.getY());
  }

  Vec2 operator-(const Vec2& v1, const Vec2& v2)
  {
    return Vec2(v1.getX() - v2.getX(), v1.getY() - v2.getY());
  }

  Vec2 operator*(const Vec2& v, double d)
  {
    return Vec2(v.getX() * d, v.getY() * d);
  }
  Vec2 operator*(double d, const Vec2& v)
  {
    return Vec2(v.getX() * d, v.getY() * d);
  }
  // Dot product
  double operator*(const Vec2& v1, const Vec2& v2)
  {
    return v1.getX() * v2.getX() + v1.getY() * v2.getY();
  }

  std::ostream& operator<<(std::ostream& out, const Vec2 v)
  {
    out << std::setprecision(3) << "Vec2(" <<
        v.getX() << ", " << v.getY() << ")";
    return out;
  }

  Vec2& Vec2::operator+=(const Vec2& rhs)
    {
    x += rhs.x;
    y += rhs.y;
    return *this;
    }

  Vec2& Vec2::operator-=(const Vec2& rhs)
    {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
    }

  Vec2& Vec2::operator*=(double factor)
    {
    x *= factor;
    y *= factor;
    return *this;
    }

  Vec2 Vec2::operator-()
  {
    return Vec2() - *this;
  }

  bool operator==(const Vec2& lhs, const Vec2& rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }

  // length comparison
  bool operator<(const Vec2& lhs, const Vec2& rhs)
  {
    return lhs.getLength() < rhs.getLength();
  }
  bool operator>(const Vec2& lhs, const Vec2& rhs)
  {
    return rhs < lhs;
  }
  bool operator<=(const Vec2& lhs, const Vec2& rhs)
  {
    return !(lhs > rhs);
  }
  bool operator>=(const Vec2& lhs, const Vec2& rhs)
  {
    return !(lhs < rhs);
  }

  // --- METHODS ---

  void Vec2::setRect(double x, double y)
  {
    this->x = x;
    this->y = y;
  }

  void Vec2::setPolar(double length, double angle)
  {
    this->x = length;
    rotateBy(angle);
  }

  void Vec2::set(Vec2 other)
  {
    setRect(other.x, other.y);
  }

  double Vec2::getLength() const
  {
    //  return sqrt(x*x + y*y);
    return hypot(x, y);
  }

  double Vec2::getAngle() const
  {
    return atan2(y, x);
  }

  bool Vec2::isZero() const
  {
    return (x == 0) && (y == 0);
  }

  void Vec2::rotateBy(double angle)
  {
    double oldX = x;
    double oldY = y;
    x = cos(angle) * oldX - sin(angle) * oldY;
    y = sin(angle) * oldX + cos(angle) * oldY;
    checkRounding();
  }

  void Vec2::rotateTo(double targetAngle)
  {
    rotateBy(targetAngle - getAngle());
  }

  void Vec2::scaleTo(double targetLength)
  {
    *this *= (targetLength / getLength());
    checkRounding();
  }

  // Uses a tolerance of 10^-12 and checks whole numbers only.
  void Vec2::checkRounding()
  {
    if (std::abs(round(x) - x) < 1.0e-12)
      x = round(x);
    if (std::abs(round(y) - y) < 1.0e-12)
      y = round(y);
  }

  /*static*/ Vec2 Vec2::mostExtreme(const std::vector<Vec2>& vectors, const Vec2& direction)
  {
    assert(!vectors.empty() && !direction.isZero());

    Vec2 farthest = vectors.front();
    double largestDot = direction * farthest;

    for (size_t i = 1; i < vectors.size(); i++)
    {
      double dot = direction * vectors.at(i);
      if (dot > largestDot)
      {
        farthest = vectors.at(i);
        largestDot = dot;
      }
    }

    return farthest;
  }
}
