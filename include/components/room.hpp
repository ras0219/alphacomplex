#pragma once

#include "components/component.hpp"
#include "point.hpp"
#include <unordered_set>

struct Room : ComponentCRTP<Component::Room, Room> {
  typedef std::unordered_set<struct Furniture*> set_t;

  Room(const Rect& r) : r(r) { }

  virtual void on_add() override;
  virtual void on_remove() override;

  inline bool contains(int tx, int ty) const {
    return (tx >= r.x)
      && (tx < r.x + r.w)
      && (ty >= r.y)
      && (ty < r.y + r.h);
  }

  std::vector<struct Furniture*> filter_furniture(struct ItemProperties const& props);

  Rect r;
  set_t furniture;
};
