/*
 * MouseHandler.h
 *
 *  Created on: Sep 8, 2018
 *      Author: Samuel Tan
 */

#ifndef MOUSEHANDLER_H_
#define MOUSEHANDLER_H_

#include <MouseReceiver.h>
#include <Vec2.h>
#include <vector>

class MouseHandler
{
  public:
    MouseHandler();
    virtual ~MouseHandler();

    void addReceiver(MouseReceiver*);
    void removeReceiver(MouseReceiver*);
    void handleMouseClick(geometry::Vec2 position, int button, int action);

    bool hasFocus(MouseReceiver*) const;

  private:
    std::vector<MouseReceiver*> receivers;
    MouseReceiver* activeReceiver;

};

#endif /* MOUSEHANDLER_H_ */
