/*
 * FlipButton.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: Samuel Tan
 */

#include <Battle.h>
#include <FlipButton.h>
#include <Renderer.h>
#include <UnitLoader.h>
#include <Values.h>
#include <Vec2.h>

namespace gui
{

  FlipButton::FlipButton(MouseHandler* mouseHandler, const geometry::Box& area,
      Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  FlipButton::~FlipButton()
  {

  }

  void FlipButton::render(graphics::Renderer& renderer) const
  {
    Button::render(renderer);

    constexpr double scale = 10;
    constexpr double angle = Values::PI*0.4;

    geometry::Box shaft(box.position, angle, scale/8, scale*5/8, -scale/2, scale);
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, shaft, Values::Depth::EMBLEMS));

    geometry::Vec2 toTip(0, scale);
    toTip.rotateBy(angle);
    geometry::Vec2 width(scale);
    width.rotateBy(angle);
    renderer.addTriangle(Values::Triangle {{
      Values::makeCV(Values::PANEL_COLOR,
          box.position + toTip - 1.0/8*width, Values::Depth::EMBLEMS),
      Values::makeCV(Values::PANEL_COLOR,
          box.position + toTip + 7.0/8*width, Values::Depth::EMBLEMS),
      Values::makeCV(Values::PANEL_COLOR,
          box.position + toTip*1.6 + 3.0/8*width, Values::Depth::EMBLEMS)
    }});

    shaft.angle += Values::PI;
    renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, shaft, Values::Depth::EMBLEMS));

    toTip *= -1;
    width *= -1;
    renderer.addTriangle(Values::Triangle {{
      Values::makeCV(Values::PANEL_COLOR,
          box.position + toTip - 1.0/8*width, Values::Depth::EMBLEMS),
      Values::makeCV(Values::PANEL_COLOR,
          box.position + toTip + 7.0/8*width, Values::Depth::EMBLEMS),
      Values::makeCV(Values::PANEL_COLOR,
          box.position + toTip*1.6 + 3.0/8*width, Values::Depth::EMBLEMS)
    }});
  }

  void FlipButton::clickAction() const
  {
    battle->getUnitLoader().flip();
  }

} /* namespace gui */
