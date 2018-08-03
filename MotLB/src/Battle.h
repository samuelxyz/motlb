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

    void add(entity::Projectile&);
    void add(entity::Particle&);
    void add(entity::Unit&);

    void remove(entity::Projectile&);
    void remove(entity::Particle&);
    void remove(entity::Unit&);

    void clearAll();

    const geometry::Box& getBounds() const;

    std::vector<entity::Unit>& getUnits();

  private:

    const geometry::Box bounds;
    std::vector<entity::Particle> particles;
    std::vector<entity::Projectile> projectiles;
    std::vector<entity::Unit> units;

    graphics::Renderer renderer;

    static constexpr Values::Color backgroundColor { 1.0f, 0.94f, 0.7f, 1.0f };

};

#endif /* BATTLE_H_ */
