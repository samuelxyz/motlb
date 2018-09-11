/*
 * GUIComponent.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Samuel Tan
 */

#include <GUIComponent.h>
#include <MouseHandler.h>
#include <Renderer.h>

namespace gui
{

  GUIComponent::GUIComponent(MouseHandler* mouseHandler, const geometry::Box& area,
      const Values::Color& color)
  : mouseHandler(mouseHandler), box(area), color(color), children()
  {
    if (mouseHandler)
      mouseHandler->addReceiver(this);
  }

  GUIComponent::~GUIComponent()
  {
    if (mouseHandler)
      mouseHandler->removeReceiver(this);
    for (GUIComponent* c : children)
      if (c)
        delete c;
  }

  void GUIComponent::addChild(GUIComponent* c)
  {
    children.emplace_back(c);
  }

  void GUIComponent::onMouseClick()
  {
    // nothing yet
  }

  void GUIComponent::update()
  {
    for (GUIComponent* c : children)
      c->update();
  }

  void GUIComponent::render(graphics::Renderer& renderer) const
  {
    renderer.addQuad(Values::makeQuad(color, box, Values::Depth::BACKGROUND));

    for (GUIComponent* c : children)
      c->render(renderer);
  }

  bool GUIComponent::handleMouseClick(geometry::Vec2 point, int button, int action)
  {
    if (box.containsAbs(point))
    {
      onMouseClick();
    }
    return false;
  }

} /* namespace gui */
