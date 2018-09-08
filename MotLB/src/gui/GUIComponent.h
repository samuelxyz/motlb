/*
 * GUIComponent.h
 *
 *  Created on: Sep 7, 2018
 *      Author: Samuel Tan
 */

#ifndef GUICOMPONENT_H_
#define GUICOMPONENT_H_

#include <Box.h>
#include <Vec2.h>
#include <vector>

#include "../Values.h"

namespace graphics
{
  class Renderer;
} /* namespace graphics */

namespace gui
{
  class GUIComponent
  {
    protected:
      geometry::Box box;
      Values::Color color;
      std::vector<GUIComponent*> children;

      virtual void onMouseClick();

    public:
      GUIComponent(const geometry::Box&, const Values::Color&);
      virtual ~GUIComponent();

      void addChild(GUIComponent*);

      void refresh();
      void render(graphics::Renderer&) const;
      void handleMouseClick(geometry::Vec2);
  };

} /* namespace gui */

#endif /* GUICOMPONENT_H_ */
