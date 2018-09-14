/*
 * Button.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <GUIComponent.h>
#include <Values.h>
#include <Vec2.h>

class Battle;

namespace gui
{

  class Button: public GUIComponent
  {
    protected:
      Battle* battle;


    public:
      Button(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~Button();

      virtual void render(graphics::Renderer&) const override;
      virtual bool handleMouseClick(geometry::Vec2 pos, int button, int action) override;
  };

} /* namespace gui */

#endif /* BUTTON_H_ */
