/*
 * GUIComponent.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Samuel Tan
 */

#include <GUIComponent.h>
#include <Renderer.h>

namespace gui
{

  GUIComponent::GUIComponent(const geometry::Box& area,
      const Values::Color& color)
  : box(area), color(color), children()
  {
  }

  GUIComponent::~GUIComponent()
  {
    for (GUIComponent* c : children)
      if (c)
        delete c;
  }

  void GUIComponent::onMouseClick()
  {
    // nothing yet
  }

  void GUIComponent::refresh()
  {
    for (GUIComponent* c : children)
      c->refresh();
  }

  void GUIComponent::render(graphics::Renderer& renderer) const
  {
    renderer.addQuad(Values::makeQuad(color, box, Values::Depth::BACKGROUND));

    for (GUIComponent* c : children)
      c->render(renderer);
  }

  void GUIComponent::handleMouseClick(geometry::Vec2 point)
  {
    if (box.containsAbs(point))
    {
      for (GUIComponent* c : children)
        c->handleMouseClick(point);

      onMouseClick();
    }
  }

} /* namespace gui */
