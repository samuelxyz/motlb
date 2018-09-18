/*
 * MouseHandler.cpp
 *
 *  Created on: Sep 8, 2018
 *      Author: Samuel Tan
 */

#include <MouseHandler.h>
#include <Window.h>

MouseHandler::MouseHandler(Window* window)
: window(window), receivers(), activeReceiver(nullptr),
  mouseHeld(false)
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
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    mouseHeld = true;
  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    mouseHeld = false;

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

void MouseHandler::update()
{
  if (mouseHeld)
  {
    handleMouseClick(window->getMousePos(), GLFW_MOUSE_BUTTON_LEFT, GLFW_REPEAT);
  }
}

bool MouseHandler::hasFocus(const MouseReceiver* receiver) const
{
  return activeReceiver == receiver;
}
