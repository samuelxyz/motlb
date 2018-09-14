/*
 * LineControls.h
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#ifndef LINECONTROLS_H_
#define LINECONTROLS_H_

#include <GUIComponent.h>

class Battle;

namespace gui
{

  class LineControls: public GUIComponent
  {
    public:
      LineControls(MouseHandler* mouseHandler, const geometry::Box& area,
          Battle* battle);
      virtual ~LineControls();

      virtual void render(graphics::Renderer&) const override;

    private:
      // buttons - unfortunately cannot be nested classes
      // and inherit from Button at the same time
      Battle* battle;
      static constexpr unsigned int NUM_BUTTONS = 4;
  };

} /* namespace gui */

#endif /* LINECONTROLS_H_ */
