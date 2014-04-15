#pragma once

#include "components/component.hpp"
#include "point.hpp"
#include <vector>

struct Furniture;

namespace item {
    struct ItemProperties;
}

struct Room : ecs::ComponentCRTP<ecs::Component::Room, Room> {
    Room(const Rect& r);

  virtual void on_add() override;
  virtual void on_remove() override;

  bool contains(int tx, int ty) const;

  std::vector<struct Furniture*> find_furniture(item::ItemProperties const& props);
  std::vector<struct Furniture*> find_furniture();

  Rect r;
};
