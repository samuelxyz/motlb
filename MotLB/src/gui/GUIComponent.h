/*
 * GUIComponent.h
 *
 *  Created on: Sep 7, 2018
 *      Author: Samuel Tan
 */

#ifndef GUICOMPONENT_H_
#define GUICOMPONENT_H_

#include <Box.h>
#include <MouseReceiver.h>
#include <Values.h>
#include <Vec2.h>
#include <vector>

class MouseHandler;

namespace graphics
{
  class Renderer;
} /* namespace graphics */

namespace gui
{
  class GUIComponent: public MouseReceiver
  {
    protected:
      MouseHandler* mouseHandler;
      geometry::Box box;
      Values::Color color;
      std::vector<GUIComponent*> children;


    public:
      GUIComponent(MouseHandler*, const geometry::Box&, const Values::Color&);
      virtual ~GUIComponent();

      void addChild(GUIComponent*);

      virtual void refresh();
      virtual void update();
      virtual void render(graphics::Renderer&) const;
      virtual bool handleMouseClick(geometry::Vec2 pos, int button, int action) override;
      virtual void clickAction() const;
  };

} /* namespace gui */

#endif /* GUICOMPONENT_H_ */
