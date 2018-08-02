/*
 * Battle.h
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#ifndef BATTLE_H_
#define BATTLE_H_

#include <vector>

#include "Box.h"
#include "entity/Particle.h"
#include "entity/Projectile.h"
#include "entity/Unit.h"
#include "render/Renderer.h"

class Battle
{

  public:

    Battle();
    ~Battle();

    void run();
    void stop();
    void update();
    void renderAll();
    void renderBox(const Values::Color& color, Box& box);

    void add(entity::Projectile&);
    void add(entity::Particle&);
    void add(entity::Unit&);

    void remove(entity::Projectile&);
    void remove(entity::Particle&);
    void remove(entity::Unit&);

    void clearAll();

    const Box& getBounds() const;

    std::vector<entity::Unit>& getUnits();

  private:

    const Box bounds;
    std::vector<entity::Particle> particles;
    std::vector<entity::Projectile> projectiles;
    std::vector<entity::Unit> units;

    render::Renderer renderer;

};

#endif /* BATTLE_H_ */
