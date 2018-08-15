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

    struct Depth
    { // kind of like a namespace substitution hack workaround thingy
      static constexpr float
      BACKGROUND  = 0.0f,
      LOWER_SMOKE = 0.2f, // problem is that transparent things
      UPPER_SMOKE = 0.2f, // also write to the depth buffer hmm
      FLASHES     = 0.2f,
      UNITS       = 0.2f,
      EMBLEMS     = 0.2f,
      PROJECTILES = 0.2f,
      TOP         = 1.0f;
    };

    struct Color
    {
        float r, g, b, a;
    };

    struct ColoredVertex
    {
        Color color;
        float x, y, z;
    };

    // For initialization: Shape s {{ }}; or Shape s = { }; because of how std::array is
    typedef std::array<ColoredVertex, 3> Triangle;
    typedef std::array<ColoredVertex, 4> Quad; // shape with four corners
    typedef std::vector<ColoredVertex> CenteredPoly; // shape with corners > 4

    static ColoredVertex makeCV(const Color& color, const geometry::Vec2& position, float z)
    {
      ColoredVertex cv
      {
        color,
        static_cast<float>(position.getX()),
        static_cast<float>(position.getY()),
        z
      };
      return cv;
    }

    static Quad makeQuad(const Color& color, const geometry::Box& box, float z)
    {
      std::array<geometry::Vec2, 4> vertices;
      box.absCorners(vertices);

      Quad quad;

      for (unsigned int i = 0; i < 4; i++)
      {
        quad[i] = makeCV( color, vertices[i], z);
      }

      return quad;
    }

    static Color interpolateColors(Color start, Color end, float fraction)
    {
      assert(0.0f <= fraction && fraction <= 1.0f && "Values::interpolateColors()");

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
