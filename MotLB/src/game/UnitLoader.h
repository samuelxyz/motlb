/*
 * UnitLoader.h
 *
 *  Created on: Aug 8, 2018
 *      Author: Samuel Tan
 */

#ifndef UNITLOADER_H_
#define UNITLOADER_H_

#include <Unit.h>
#include <Vec2.h>
#include <vector>

class Battle;

namespace graphics
{
  class Renderer;
}

class UnitLoader
{
  public:
    UnitLoader(Battle& battle);
    ~UnitLoader();

    void render(graphics::Renderer& renderer);

    void flip();
    void increment();
    void decrement();
    void refresh();
    void addAndClearAll();
    void cancel();

    bool processClick(geometry::Vec2 position);
    bool isLineStarted();

    static entity::Unit* make(Battle* battle, entity::Entity::Team team,
        geometry::Vec2 position, double angle, int type);

  private:
    entity::Unit* make(geometry::Vec2 position);
    entity::Unit* make(geometry::Vec2 position, double angle);
    void startLine(geometry::Vec2 start, geometry::Vec2 end);
    void clearStagedUnits();
    void drawBar(graphics::Renderer& renderer,
        geometry::Vec2 start, geometry::Vec2 end) const;

    Battle& battle;
    std::vector<entity::Unit*> stagedUnits;
    unsigned int count;
    bool alreadyClicked, lineStarted;
    geometry::Vec2 v1, v2, facing;
};

#endif /* UNITLOADER_H_ */
