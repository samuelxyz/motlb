/*
 * MouseHandler.cpp
 *
 *  Created on: Sep 8, 2018
 *      Author: Samuel Tan
 */

#include <MouseHandler.h>


MouseHandler::MouseHandler()
: receivers(), activeReceiver(nullptr)
{
}

MouseHandler::~MouseHandler()
{
}

void MouseHandler::addReceiver(MouseReceiver* receiver)
{
  receivers.emplace_back(receiver);
}

void MouseHandler::removeReceiver(const MouseReceiver* receiver)
{
  for (unsigned int i = 0; i < receivers.size();)
  {
    if (receivers[i] == receiver)
      receivers.erase(receivers.begin() + i);
    else
      ++i;
  }
}

void MouseHandler::handleMouseClick(geometry::Vec2 position, int button, int action)
{
  if (activeReceiver != nullptr)
  {
    if (!activeReceiver->handleMouseClick(position, button, action))
      activeReceiver = nullptr; // lost focus
  }
  else
  {
    for (MouseReceiver* receiver : receivers)
    {
      if (receiver->handleMouseClick(position, button, action))
      {
        activeReceiver = receiver;
        break;
      }
    }
  }
}

bool MouseHandler::hasFocus(const MouseReceiver* receiver) const
{
  return activeReceiver == receiver;
}
