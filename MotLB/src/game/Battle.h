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
#include <MouseReceiver.h>
#include <Particle.h>
#include <Projectile.h>
#include <Unit.h>
#include <Values.h>
#include <Vec2.h>
#include <vector>
#include <UnitLoader.h>

namespace gui {
  class StartStopButton;
  class ModeSelector;
  class ColorSelector;
  class TypeSelector;
}

class Window;

class Battle: public MouseReceiver
{
  public:

    Battle(Window*);
    ~Battle();

    void refreshGUI();
    void update();
    void render(graphics::Renderer&);

    void add(entity::Projectile*);
    void add(entity::Particle*);
    void add(entity::Unit*);

    bool remove(entity::Unit*);

    void clearAll();
    void resurrectAllIfDead();
    void healAllIfAlive();

    void handleKeypress(int key, int action);
    virtual bool handleMouseClick(geometry::Vec2 position, int button, int action) override;

    const geometry::Box& getBounds() const { return bounds; }

    std::vector<entity::Unit*>& getUnits() { return units; }
    std::vector<entity::Projectile*>& getProjectiles() { return projectiles; }
    std::vector<entity::Particle*>& getParticles() { return particles; }

    void updateWindowTitle();

  private:

    Window* window;
    const geometry::Box bounds;

    // heap allocated
    std::vector<entity::Particle*> particles;
    std::vector<entity::Projectile*> projectiles;
    std::vector<entity::Unit*> units;

    friend class gui::StartStopButton;
    friend class gui::ModeSelector;
    friend class gui::ColorSelector;
    friend class gui::TypeSelector;
    friend class UnitLoader;
    UnitLoader unitLoader;

    entity::Entity::Team selectedTeam;
    enum class UnitType
    {
        UNIT     = 0,
        GUNNER   = 1,
        LAUNCHER = 2,
        CANNON   = 3,
        RAILGUN  = 4,
        GIANT    = 5,
        SHIELD   = 6,
        RACER    = 7,
        ACE      = 8
    } selectedUnitType;
    enum class BattleAction
    {
        DELETE = 0,
        SINGLE = 1,
        LINE   = 2
    } selectedAction;

    bool paused;

    static constexpr Values::Color backgroundColor
    {
//      1.0f, 0.94f, 0.7f, 1.0f
      0.7f, 0.7f, 0.7f, 1.0f
    };

  public:
    UnitLoader& getUnitLoader() { return unitLoader; }

};

#endif /* BATTLE_H_ */
