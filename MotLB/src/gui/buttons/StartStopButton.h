/*
 * StartStopButton.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#ifndef STARTSTOPBUTTON_H_
#define STARTSTOPBUTTON_H_

#include <Button.h>
#include <Vec2.h>

class Battle;

namespace gui
{

  class StartStopButton: public Button
  {
    public:
      StartStopButton(MouseHandler*, const geometry::Box&, Battle*);
      virtual ~StartStopButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* STARTSTOPBUTTON_H_ */
