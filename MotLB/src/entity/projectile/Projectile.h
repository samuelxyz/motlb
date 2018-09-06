/*
 * Projectile.h
 *
 *  Created on: Jul 24, 2018
 *      Author: Samuel Tan
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include <Entity.h>
#include <Vec2.h>

namespace entity
{
  class Unit;

  // single hit
  class Projectile: public Entity
  {
    public:

      Projectile(Battle*, Team, geometry::Vec2 position, geometry::Vec2 velocity,
          double damage, double knockback, bool friendlyFire,
          float depth = Values::Depth::PROJECTILES);
      virtual ~Projectile();

      virtual void update() override;
      virtual void render(graphics::Renderer& renderer) const override;

      friend class Shield;

    protected:

      double damage, inertia;
      bool friendlyFire;

      void checkHit();
      virtual void hit(Unit&);
      virtual void explode(geometry::Vec2 velocity = geometry::Vec2());
      virtual void rebound(double normalAngle, double randomAngle);

      virtual void move() override;
      virtual void checkContainment() override;

      enum class Mode
      {
        FLYING,
        FADE_OUT
      } mode;

      enum class BlockingBehavior
      {
          EXPLODE_AND_REBOUND,
          REBOUND_ONLY,
          EXPLODE_ONLY,
          UNBLOCKABLE
      } blockingBehavior;

      int renderLength;

    private:

      void doFading();

  };

} /* namespace entity */

#endif /* PROJECTILE_H_ */
