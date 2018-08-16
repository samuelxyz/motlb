/*
 * Vec2.h
 *
 *  Created on: Jul 12, 2018
 *      Author: xinyi
 */

#ifndef VEC2_H_
#define VEC2_H_

#include <iostream>
#include <vector>

namespace geometry
{
  struct Vec2
  { // small and fundamental enough to pass by value

      double x, y;

      Vec2(double x = 0, double y = 0);
      // copy constructor is default

      friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
      friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
      friend Vec2 operator*(const Vec2& v, double d);
      friend Vec2 operator*(double d, const Vec2& v);

      Vec2& operator+=(const Vec2& rhs);
      Vec2& operator-=(const Vec2& rhs);
      Vec2& operator*=(double factor);
      Vec2 operator-();
      // operator= has a good default

      // dot product
      friend double operator*(const Vec2& v1, const Vec2& v2);

      friend std::ostream& operator<<(std::ostream& out, const Vec2& v);

      friend bool operator==(const Vec2&, const Vec2&);

      // comparing magnitudes
      friend bool operator<(const Vec2&, const Vec2&);
      friend bool operator>(const Vec2&, const Vec2&);
      friend bool operator<=(const Vec2&, const Vec2&);
      friend bool operator>=(const Vec2&, const Vec2&);

      // mutators
      void setPolar(double length, double angle);
      void rotateBy(double angle);
      void rotateTo(double targetAngle);
      void scaleTo(double targetLength);

      // accessors
      double getLength() const;
      double getAngle() const;
      bool isZero() const;

//      // misc
//      static Vec2& mostExtreme(const std::vector<Vec2>&, const Vec2& direction);
  };
}
#endif /* VEC2_H_ */
