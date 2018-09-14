/*
 * FlipButton.h
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#ifndef FLIPBUTTON_H_
#define FLIPBUTTON_H_

#include <Button.h>

namespace gui
{

  class FlipButton: public Button
  {
    public:
      FlipButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~FlipButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* FLIPBUTTON_H_ */
