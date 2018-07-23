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

class Vec2
{ // small and fundamental enough to pass by value
  public:
    Vec2(double x = 0, double y = 0);
    // copy constructor is default

    // operator overrides
    // TODO: maybe change friend functions to take advantage of friendship?
    friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
    friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
    friend Vec2 operator*(const Vec2& v, double d);
    friend Vec2 operator*(double d, const Vec2& v);
    friend std::ostream& operator<<(std::ostream& out, const Vec2 v);
    Vec2& operator+=(const Vec2& rhs);
    Vec2& operator-=(const Vec2& rhs);
    Vec2& operator*=(double factor);
    Vec2 operator-();
    // operator= has a good default
    friend bool operator==(const Vec2&, const Vec2&);
    // comparing magnitudes
    friend bool operator<(const Vec2&, const Vec2&);
    friend bool operator>(const Vec2&, const Vec2&);
    friend bool operator<=(const Vec2&, const Vec2&);
    friend bool operator>=(const Vec2&, const Vec2&);

    // methods
    void setRect(double x, double y);
    void setPolar(double length, double angle);
    void set(Vec2 other);
    double getX() const {return x;}
    double getY() const {return y;}
    double getLength() const;
    double getAngle() const;

    bool isZero() const;

    void rotateBy(double angle);
    void rotateTo(double targetAngle);
    void scaleTo(double targetLength);

    static Vec2 mostExtreme(const std::vector<Vec2>&, const Vec2& direction);

  private:

    double x, y;

    void checkRounding();
};

#endif /* VEC2_H_ */
