/*
 * Values.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Samuel Tan
 */

#ifndef VALUES_H_
#define VALUES_H_

#include <Box.h>
#include <Vec2.h>
#include <array>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>

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

//        Color(float r=0.0f, float g=0.0f, float b=0.0f, float a=0.0f)
//        : r(r), g(g), b(b), a(a)
//        {
//        }
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

    static Color interpolateColors(Color start, Color end, float fraction)
    {
      assert(0.0f <= fraction && fraction <= 1.0f);

      // shortcuts
      if (fraction == 1.0f)
        return end;
      if (fraction == 0.0f)
        return start;

      return Color {
        start.r * (1-fraction) + end.r * fraction,
        start.g * (1-fraction) + end.g * fraction,
        start.b * (1-fraction) + end.b * fraction,
        start.a * (1-fraction) + end.a * fraction
      };
    }

    static double random()
    {
      return static_cast<double>(rand())/RAND_MAX;
    }

    static double random(double min, double max)
    {
      double intervalSize = max-min;
      return min + (random() * intervalSize);
    }

    static void initRand()
    {
      srand(time(nullptr));
    }
};

#endif /* VALUES_H_ */