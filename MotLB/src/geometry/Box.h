/*
 * Box.h
 *
 *  Created on: Jul 16, 2018
 *      Author: xinyi
 */

#ifndef BOX_H_
#define BOX_H_

#include <array>
#include <cassert>

#include "Vec2.h"

namespace geometry
{

  class Box
  {
    public:
      Box(geometry::Vec2 position, double angle, double xMin, double xMax,
          double yMin, double yMax);
      Box(geometry::Vec2 corner1, geometry::Vec2 corner2);

      friend std::ostream& operator<<(std::ostream& out, const Box&);

      geometry::Vec2 position;
      double angle;

      double getWidth() const;
      double getHeight() const;

      geometry::Vec2 toRelative(geometry::Vec2 absPoint) const;
      geometry::Vec2 toAbs(geometry::Vec2 relPoint) const;
      bool containsAbs(geometry::Vec2 absPoint) const;
      void absCorners(std::array<geometry::Vec2, 4>& toFill) const;

      geometry::Vec2 toClosestEdge(geometry::Vec2 absPoint) const; // rel directions
      static geometry::Vec2 collide(const Box& standOn, const Box& giveWay);
      geometry::Vec2 contain(const Box& target) const;

//      static geometry::Vec2 satCollide(const Box& standOn, const Box& giveWay);
      // ^^ now renamed to simply collide
//      static bool overlaps(const Box&, const Box&);

    private:

      double xMin, xMax, yMin, yMax;

      bool containsRel(geometry::Vec2 relPoint) const;
      void relCorners(std::array<geometry::Vec2, 4>& toFill) const;

      geometry::Vec2 collideOneWay(const Box& other) const;

//      bool overlapsOneWay(const Box& other) const; // if other inside this
//      geometry::Vec2 satOneWay(const Box& other) const;
      // now renamed to simply collideOneWay

      struct Interval
      {
          double min, max;

          Interval(double min, double max): min(min), max(max)
          {
            assert(min <= max);
          }
      };

      static double collide1D(const Interval standOn, const Interval giveWay);
  };
}

#endif /* BOX_H_ */
