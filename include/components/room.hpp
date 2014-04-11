#pragma once

#include "components/component.hpp"
#include <unordered_set>

struct Room : ComponentCRTP<Component::Room, Room> {
  typedef std::unordered_set<struct Furniture*> set_t;

  Room(const struct Rect& r);

  virtual void on_add() override;
  virtual void on_remove() override;

  inline bool contains(int tx, int ty) const;

  std::vector<struct Furniture*> filter_furniture(struct ItemProperties const& props);

  const struct Rect& r;
  set_t furniture;
};
