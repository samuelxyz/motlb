/*
 * HealButton.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#ifndef HEALBUTTON_H_
#define HEALBUTTON_H_

#include <Button.h>

namespace gui
{

  class HealButton: public Button
  {
    public:
      HealButton(MouseHandler* mouseHandler,
          const geometry::Box& area, Battle* battle);
      virtual ~HealButton();

      virtual void render(graphics::Renderer&) const override;
      virtual void clickAction() const override;
  };

} /* namespace gui */

#endif /* HEALBUTTON_H_ */
