#pragma once

#include "entities/entity.hpp"
#include "city.hpp"

struct PositionComp : AspectStatic<Aspect::Position, PositionComp> {
  PositionComp(struct Position p) : pos(p) { }

  inline City& city() { return *pos.city; }
  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }
  inline point as_point() const { return pos.as_point(); }

  inline void move(int tx, int ty) {
    remove();
    pos.x = tx;
    pos.y = ty;
    insert();
  }
  inline void move(struct Position tp) {
    remove();
    pos.x = tp.x;
    pos.y = tp.y;
    insert();
  }

private:
  // These methods should not normally be called.
  inline void insert() { city().add_ent(x(), y(), parent); }
  inline void remove() { city().del_ent(x(), y(), parent); }

  struct Position pos;
};
