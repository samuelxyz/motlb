/*
 * Battle.h
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#ifndef BATTLE_H_
#define BATTLE_H_

#include <vector>

#include "entity/Particle.h"
#include "entity/Projectile.h"
#include "entity/Unit.h"
#include "geometry/Box.h"
#include "graphics/Renderer.h"
#include "Values.h"

class Battle
{
  public:

    Battle();
    ~Battle();

    void run();
    void stop();
    void update();

    void renderAll();

    // TODO: make sure these are all heap allocated
    void add(entity::Projectile*);
    void add(entity::Particle*);
    void add(entity::Unit*);

    bool remove(entity::Projectile*);
    bool remove(entity::Particle*);
    bool remove(entity::Unit*);

    void clearAll();

    const geometry::Box& getBounds() const;

    std::vector<entity::Unit*>& getUnits();

  private:

    const geometry::Box bounds;

    // heap allocated
    std::vector<entity::Particle*> particles;
    std::vector<entity::Projectile*> projectiles;
    std::vector<entity::Unit*> units;

    graphics::Renderer renderer;

    static constexpr Values::Color backgroundColor
    {
//      1.0f, 0.94f, 0.7f, 1.0f
      0.7f, 0.7f, 0.7f, 1.0f
    };

};

#endif /* BATTLE_H_ */
