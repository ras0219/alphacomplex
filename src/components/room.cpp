#include "components/room.hpp"
#include "city.hpp"
#include "components/furniture.hpp"
#include "components/item.hpp"
#include "entities/entity.hpp"
#include <algorithm>

Room::Room(const Rect& r) : r(r) {}

bool Room::contains(int tx, int ty) const {
  return (tx >= r.x)
    && (tx < r.x + r.w)
    && (ty >= r.y)
    && (ty < r.y + r.h);
}

std::vector<Furniture*> Room::find_furniture(ItemProperties const& props) {
  std::vector<struct Furniture*> ret = find_furniture();
  auto pred = [&props](struct Furniture* f) {
    if (auto i = f->parent->has_get<struct Item>())
      return &i->prop == &props;
    else
        {

        }
      return false;
  };
    auto it = std::partition(ret.begin(), ret.end(), pred);
    ret.erase(it, ret.end());
  return ret;
}

std::vector<Furniture*> Room::find_furniture() {
    return r.city->find_furniture(r);
}

void Room::on_add() {
  r.city->add_room(this);
}

void Room::on_remove() {
  r.city->del_room(this);
}
