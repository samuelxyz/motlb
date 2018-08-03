/*
 * Values.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Samuel Tan
 */

#ifndef VALUES_H_
#define VALUES_H_

#include "geometry/Box.h"

class Values
{
  public:
    static constexpr unsigned int BATTLE_WIDTH  = 800,
                                  BATTLE_HEIGHT = 800;
    static constexpr double PI      = 3.141592653589793,
                            TWO_PI  = 6.283185307179586,
                            HALF_PI = 1.570796326794897;

    struct Color
    {
        float r, g, b, a;
    };

    struct ColoredVertex
    {
        Color color;
        float x, y;
    };

    typedef std::array<ColoredVertex, 4> Quad; // shape with four corners

    typedef std::vector<ColoredVertex> CenteredPoly; // shape with corners > 4

    static Quad makeQuad(const Color& color, const geometry::Box& box)
    {
      std::array<geometry::Vec2, 4> vertices;
      box.absCorners(vertices);

      Quad quad;

      for (unsigned int i = 0; i < 4; i++)
      {
        quad[i] =
        {
            color,
            static_cast<float>(vertices[i].getX()),
            static_cast<float>(vertices[i].getY())
        };
      }

      return quad;
    }
};

#endif /* VALUES_H_ */
