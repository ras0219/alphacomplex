#pragma once

#include "entities/entity.hpp"
#include "city.hpp"

struct Position : ComponentCRTP<Component::Position, Position> {
  Position(Point p) : pos(p) { }

  inline City& city() { return *pos.city; }
  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }
  inline Point as_point() const { return pos; }

  inline void move(int tx, int ty) {
    remove();
    pos.x = tx;
    pos.y = ty;
    insert();
  }
  inline void move(Point tp) {
    remove();
    pos.x = tp.x;
    pos.y = tp.y;
    insert();
  }

private:
  inline void insert() { city().add_ent(x(), y(), parent); }
  inline void remove() { city().del_ent(x(), y(), parent); }

  Point pos;
};
