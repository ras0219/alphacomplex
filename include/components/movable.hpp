#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"
#include "position.hpp"

struct Movable : AspectStatic<Aspect::Movable, Movable> {
  Movable(struct Position p) : pos(p) { }

  inline City& city() { return *pos.city; }
  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }
  inline point as_point() const { return pos.as_point(); }

  inline void set(int x, int y) {
    pos.x = x;
    pos.y = y;
  }
  inline void set(point p) {
    pos.x = p.first;
    pos.y = p.second;
  }

  struct Position pos;
};

struct SimpleMovableSystem : SubSystem<SimpleMovableSystem, Movable> {
inline void update_item(Ent* e, Movable* m) {
    PositionComp* pc = e->assert_get<PositionComp>();
    pc->move(m->pos);
  }
};

extern SimpleMovableSystem smsystem;
