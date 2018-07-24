/*
 * Box.h
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#ifndef BOX_H_
#define BOX_H_

#include <array>

#include "Vec2.h"

class Box
{
  public:
    Box(Vec2 position, double angle, double xMin, double xMax,
        double yMin, double yMax);
    Box(Vec2 corner1, Vec2 corner2);

    friend std::ostream& operator<<(std::ostream& out, const Box&);

    Vec2 position;
    double angle;

    double getWidth() const;
    double getHeight() const;

    Vec2 toRelative(Vec2 absPoint) const;
    Vec2 toAbs(Vec2 relPoint) const;
    bool containsAbs(Vec2 absPoint) const;
    void absCorners(std::array<Vec2, 4>& toFill) const;

    Vec2 toClosestEdge(Vec2 absPoint) const; // rel directions
    static bool overlaps(const Box&, const Box&);
    static Vec2 collide(const Box& standOn, const Box& giveWay);
    Vec2 contain(const Box& target) const;

  private:

    double xMin, xMax, yMin, yMax;

    bool containsRel(Vec2 relPoint) const;
    void relCorners(std::array<Vec2, 4>& toFill) const;

    bool overlapsOneWay(const Box& other) const; // if other inside this
    Vec2 collideOneWay(const Box& other) const;
};

#endif /* BOX_H_ */
