#include "components/room.hpp"
#include "city.hpp"
#include "components/furniture.hpp"
#include "components/item.hpp"
#include "entities/entity.hpp"

Room::Room(const Rect& r) : r(r) {}

bool Room::contains(int tx, int ty) const {
  return (tx >= r.x)
    && (tx < r.x + r.w)
    && (ty >= r.y)
    && (ty < r.y + r.h);
}

std::vector<Furniture*> Room::filter_furniture(ItemProperties const& props) {
  std::vector<struct Furniture*> ret;
  auto pred = [&props](struct Furniture* f) {
    if (auto i = f->parent->has_get<struct Item>())
      return &i->prop == &props;
    else
      return false;
  };
  std::copy_if(furniture.begin(), furniture.end(), std::back_inserter(ret), pred);
  return ret;
}

void Room::on_add() {
  r.city->add_room(this);
}

void Room::on_remove() {
  r.city->del_room(this);
}
