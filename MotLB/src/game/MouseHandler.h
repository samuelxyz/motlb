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

class Window;

class MouseHandler
{
  public:
    MouseHandler(Window* window);
    virtual ~MouseHandler();

    void addReceiver(MouseReceiver*);
    void removeReceiver(const MouseReceiver*);
    void handleMouseClick(geometry::Vec2 position, int button, int action);
    void update();

    bool hasFocus(const MouseReceiver*) const;

  private:
    Window* window;
    std::vector<MouseReceiver*> receivers;
    MouseReceiver* activeReceiver;
    bool mouseHeld;

};

#endif /* MOUSEHANDLER_H_ */
