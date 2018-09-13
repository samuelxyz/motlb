/*
 * TypeSelector.h
 *
 *  Created on: Sep 11, 2018
 *      Author: Samuel Tan
 */

#ifndef TYPESELECTOR_H_
#define TYPESELECTOR_H_

#include <GUIComponent.h>
#include <Unit.h>
#include <Vec2.h>

class Battle;

namespace gui
{

  class TypeSelector: public GUIComponent
  {
    private:
      Battle* battle;
      std::vector<entity::Unit*> units;
      std::vector<int> types;
      std::vector<geometry::Box> boxes;
      const geometry::Vec2 start;

      double unitAngle;

      static constexpr int NUM_TYPES = 9;
      static constexpr double ROTATION_RATE = 0.02;
      static const geometry::Vec2 dx;

    public:
      static constexpr int BOX_SIZE = 80;
      static constexpr int BOX_LIMIT_Y = 7;

      TypeSelector(MouseHandler*, geometry::Vec2 start, Battle*);
      virtual ~TypeSelector();

      virtual void refresh() override;
      virtual void update() override;
      virtual void render(graphics::Renderer&) const override;
      virtual bool handleMouseClick(geometry::Vec2 position, int button,
          int action) override;
  };

} /* namespace gui */

#endif /* TYPESELECTOR_H_ */
