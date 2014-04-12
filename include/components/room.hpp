#pragma once

#include "components/component.hpp"
#include "point.hpp"
#include <vector>

struct Room : ComponentCRTP<Component::Room, Room> {
  Room(const Rect& r) : r(r) { }

  virtual void on_add() override;
  virtual void on_remove() override;

  inline bool contains(int tx, int ty) const {
    return (tx >= r.x)
      && (tx < r.x + r.w)
      && (ty >= r.y)
      && (ty < r.y + r.h);
  }

  std::vector<struct Furniture*> find_furniture(struct ItemProperties const& props);
  std::vector<struct Furniture*> find_furniture();

  Rect r;
};
