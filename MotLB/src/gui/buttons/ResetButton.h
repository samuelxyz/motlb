/*
 * ResetButton.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#ifndef RESETBUTTON_H_
#define RESETBUTTON_H_

#include <Button.h>

namespace gui
{

  class ResetButton: public Button
  {
    public:
      ResetButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~ResetButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* RESETBUTTON_H_ */
