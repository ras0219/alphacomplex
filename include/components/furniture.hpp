#pragma once

#include "components/component.hpp"
#include "point.hpp"
#include <unordered_set>

struct Furniture : ComponentCRTP<Component::Furniture, Furniture> {
  using set_t = std::unordered_set<struct Room*>;
  Furniture(const Point& pos) : pos(pos) {}
  ~Furniture() { deinit(); }

  void init();
  void deinit();

  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }

  Point pos;
  set_t rooms;
};
