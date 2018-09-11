/*
 * ColorSelector.h
 *
 *  Created on: Sep 11, 2018
 *      Author: Samuel Tan
 */

#ifndef COLORSELECTOR_H_
#define COLORSELECTOR_H_

#include <GUIComponent.h>
#include <Vec2.h>

class Battle;

namespace gui
{

  class ColorSelector: public GUIComponent
  {
    private:
      Battle* battle;

      static constexpr unsigned int NUM_SECTIONS = 4;
      static constexpr unsigned int HIGHLIGHT_THICKNESS = 4;
      const double sectionWidth;

      std::array<geometry::Box, NUM_SECTIONS> boxes;
      mutable geometry::Box highlight; // bs, I can't modify my own members by default?!
      geometry::Vec2 dx, left;

    public:
      ColorSelector(MouseHandler*, const geometry::Box&, Battle*);
      virtual ~ColorSelector();

      virtual void render(graphics::Renderer&) const;
      virtual bool handleMouseClick(geometry::Vec2 pos, int button, int action) override;
  };

} /* namespace gui */

#endif /* COLORSELECTOR_H_ */
