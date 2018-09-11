/*
 * ResurrectButton.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#ifndef RESURRECTBUTTON_H_
#define RESURRECTBUTTON_H_

#include <Button.h>

namespace gui
{

  class ResurrectButton: public Button
  {
    public:
      ResurrectButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~ResurrectButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* RESURRECTBUTTON_H_ */
