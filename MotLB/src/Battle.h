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
#include "Particle.h"

class Entity;

using namespace entity;

class Battle
{
  public:

    Battle();
    ~Battle();

    void run();
    void stop();

    void add(Entity&);
    void add(Particle&);
    void remove(Entity&);
    void remove(Particle&);
    void clearAll();

    const Box& getBounds() const;

  private:

    const Box bounds;
    std::vector<Particle> particles;
    void update();
    void render();
};

#endif /* BATTLE_H_ */
