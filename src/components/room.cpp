#include "components/room.hpp"
#include "city.hpp"
#include "components/furniture.hpp"
#include "components/item.hpp"
#include "entities/entity.hpp"

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
