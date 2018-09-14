/*
 * DecrementButton.h
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#ifndef DECREMENTBUTTON_H_
#define DECREMENTBUTTON_H_

#include <Button.h>

namespace gui
{

  class DecrementButton: public Button
  {
    public:
      DecrementButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~DecrementButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* DECREMENTBUTTON_H_ */
