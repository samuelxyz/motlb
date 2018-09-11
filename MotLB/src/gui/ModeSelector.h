/*
 * ModeSelector.h
 *
 *  Created on: Sep 11, 2018
 *      Author: Samuel Tan
 */

#ifndef MODESELECTOR_H_
#define MODESELECTOR_H_

#include <GUIComponent.h>
#include <Vec2.h>

class Battle;

namespace gui
{

  class ModeSelector: public GUIComponent
  {
    private:
      Battle* battle;

      static constexpr unsigned int NUM_MODES = 3;
      const double sectionWidth;

      geometry::Vec2 left, dx;
      std::array<geometry::Box, NUM_MODES> boxes;

    public:
      ModeSelector(MouseHandler*, const geometry::Box&, Battle*);
      virtual ~ModeSelector();

      virtual void render(graphics::Renderer&) const override;
      virtual bool handleMouseClick(geometry::Vec2 pos, int button,
          int action) override;
  };

} /* namespace gui */

#endif /* MODESELECTOR_H_ */
