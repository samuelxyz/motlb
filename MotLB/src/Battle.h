/*
 * Battle.h
 *
 *  Created on: Jul 19, 2018
 *      Author: xinyi
 */

#ifndef BATTLE_H_
#define BATTLE_H_

#include <Box.h>
#include <Entity.h>
#include <Particle.h>
#include <Projectile.h>
#include <Renderer.h>
#include <Unit.h>
#include <vector>

#include "UnitLoader.h"
#include "Values.h"

class Window;

class Battle
{
  public:

    Battle(Window*);
    ~Battle();

    void update();
    void renderAll();

    void add(entity::Projectile*);
    void add(entity::Particle*);
    void add(entity::Unit*);

    bool remove(entity::Projectile*);
    bool remove(entity::Particle*);
    bool remove(entity::Unit*);

    void clearAll();
    void resurrectAllIfDead();
    void healAllIfAlive();

    void handleKeypress(int key, int action);
    void handleMouseClick(int button, int action, double x, double y);

    const geometry::Box& getBounds() const;

    std::vector<entity::Unit*>& getUnits();
    std::vector<entity::Projectile*>& getProjectiles();

  private:

    void updateWindowTitle();

  private:

    Window* window;
    const geometry::Box bounds;

    // heap allocated
    std::vector<entity::Particle*> particles;
    std::vector<entity::Projectile*> projectiles;
    std::vector<entity::Unit*> units;

    graphics::Renderer renderer;

    friend class UnitLoader;
    UnitLoader unitLoader;

    entity::Entity::Team selectedTeam;
    enum class UnitType
    {
        UNIT,
        GUNNER,
        LAUNCHER,
        CANNON,
        RAILGUN,
        GIANT,
        SHIELD
    } selectedUnitType;
    enum class BattleAction
    {
        SINGLE,
        LINE,
        DELETE
    } selectedAction;

    bool paused;

    static constexpr Values::Color backgroundColor
    {
//      1.0f, 0.94f, 0.7f, 1.0f
      0.7f, 0.7f, 0.7f, 1.0f
    };

};

#endif /* BATTLE_H_ */
