/*
 * ConfirmButton.h
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#ifndef CONFIRMBUTTON_H_
#define CONFIRMBUTTON_H_

#include <Button.h>

namespace gui
{

  class ConfirmButton: public Button
  {
    public:
      ConfirmButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~ConfirmButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* CONFIRMBUTTON_H_ */
