/*
 * Launcher.h
 *
 *  Created on: Aug 10, 2018
 *      Author: Samuel Tan
 */

#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include <Gunner.h>

namespace entity
{

  class Launcher: public Gunner
  {
    public:
      Launcher(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity, double angle);
      virtual ~Launcher();

      virtual void render(graphics::Renderer&) const override;

    protected:
      virtual void attack() override;
  };

} /* namespace entity */

#endif /* LAUNCHER_H_ */
