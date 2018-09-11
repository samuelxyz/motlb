/*
 * StartStopButton.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: Samuel Tan
 */

#include <Box.h>
#include <Renderer.h>
#include <StartStopButton.h>
#include <Values.h>
#include <Battle.h>

namespace gui
{

  StartStopButton::StartStopButton(MouseHandler* mouseHandler,
      const geometry::Box& area, Battle* battle)
  : Button(mouseHandler, area, battle)
  {
  }

  StartStopButton::~StartStopButton()
  {
  }

  void StartStopButton::render(graphics::Renderer& renderer) const
  {
    using geometry::Vec2;

    Button::render(renderer);

    constexpr double scale = 10.0;

    if (battle->paused)
    {
      // play button
      renderer.addTriangle(Values::Triangle {{
        Values::makeCV(Values::PANEL_COLOR, box.position + Vec2(-scale,  scale), Values::Depth::EMBLEMS),
        Values::makeCV(Values::PANEL_COLOR, box.position + Vec2(-scale, -scale), Values::Depth::EMBLEMS),
        Values::makeCV(Values::PANEL_COLOR, box.position + Vec2( scale,      0), Values::Depth::EMBLEMS)
      }});
    }
    else
    {
      // pause button
      constexpr double boxHalfWidth  = scale/3;
      constexpr double boxHalfHeight = scale;

      geometry::Vec2 dx;
      dx.setPolar(boxHalfWidth * 2, 0);

      geometry::Box right = geometry::Box(box.position + dx, 0,
          -boxHalfWidth, boxHalfWidth, -boxHalfHeight, boxHalfHeight);
      geometry::Box left  = geometry::Box(box.position - dx, 0,
          -boxHalfWidth, boxHalfWidth, -boxHalfHeight, boxHalfHeight);

      renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, right, Values::Depth::EMBLEMS));
      renderer.addQuad(Values::makeQuad(Values::PANEL_COLOR, left,  Values::Depth::EMBLEMS));
    }
  }

  void StartStopButton::clickAction() const
  {
    battle->paused = !battle->paused;
    battle->updateWindowTitle();
  }

} /* namespace gui */
