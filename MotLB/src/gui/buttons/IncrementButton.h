/*
 * IncrementButton.h
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#ifndef INCREMENTBUTTON_H_
#define INCREMENTBUTTON_H_

#include <Button.h>

namespace gui
{

  class IncrementButton: public Button
  {
    public:
      IncrementButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~IncrementButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* INCREMENTBUTTON_H_ */
