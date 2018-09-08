/*
 * MouseReceiver.h
 *
 *  Created on: Sep 8, 2018
 *      Author: Samuel Tan
 */

#ifndef MOUSERECEIVER_H_
#define MOUSERECEIVER_H_

#include <Vec2.h>

class MouseReceiver
{
  public:
    MouseReceiver(){};
    virtual ~MouseReceiver(){};

    virtual bool handleMouseClick(geometry::Vec2 position, int button, int action) = 0;
};

#endif /* MOUSERECEIVER_H_ */
