#pragma once

#include "components/component.hpp"
#include "entities/subsystem.hpp"
#include "position.hpp"

struct City;

struct Movable : ComponentCRTP<Component::Movable, Movable> {
  Movable(Point p) : pos(p) { }

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

  Point pos;
};

struct SimpleMovableSystem : SubSystem<SimpleMovableSystem, Movable, Position> {
  inline void update_item(struct Ent*, Movable* m, Position* pc) {
    pc->move(m->pos);
  }
};

extern SimpleMovableSystem smsystem;
