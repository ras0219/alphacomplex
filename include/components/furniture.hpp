#pragma once

#include "components/component.hpp"
#include "point.hpp"
#include <unordered_set>

struct Furniture : ComponentCRTP<Component::Furniture, Furniture> {
  using set_t = std::unordered_set<struct Room*>;
  Furniture(const Point& pos) : pos(pos) {}

  virtual void on_add() override;
  virtual void on_remove() override;

  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }
  inline Point as_point() const { return pos; }

  Point pos;
  set_t rooms;
};
