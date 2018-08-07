/*
 * Projectile.h
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

namespace entity
{

  class Unit;

  // single hit
  class Projectile: public Entity
  {
    public:

      Projectile(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity,
          double damage, double knockback, double friendlyFire);
      virtual ~Projectile();

      virtual void update() override;
      virtual void render(graphics::Renderer& renderer) const override;

    protected:

      double damage, inertia;
      bool friendlyFire;

      void checkHit();
      virtual void hit(Unit&);

      virtual void move() override;
      virtual void checkContainment() override;

    private:

      enum class Mode
      {
        FLYING,
        FADE_OUT
      } mode;
      int renderLength;

      void checkFading();

  };

} /* namespace entity */

#endif /* PROJECTILE_H_ */
