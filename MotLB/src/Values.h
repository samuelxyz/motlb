/*
 * Values.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Samuel Tan
 */

#ifndef VALUES_H_
#define VALUES_H_

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
};

#endif /* VALUES_H_ */
