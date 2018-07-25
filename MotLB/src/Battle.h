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

class Battle
{

  public:

    Battle();
    ~Battle();

    void run();
    void stop();

    template <class... Args>
    void add(entity::Entity::Type, Args&&...);

    void remove(entity::Projectile&);
    void remove(entity::Particle&);
    void remove(entity::Unit&);

//    // dummy for Entity.checkContainment()
//    void remove(entity::Entity&);

    void clearAll();

    const Box& getBounds() const;

    std::vector<entity::Unit>& getUnits();

  private:

    const Box bounds;
    std::vector<entity::Particle> particles;
    std::vector<entity::Projectile> projectiles;
    std::vector<entity::Unit> units;
    void update();
    void render();
};

#endif /* BATTLE_H_ */
