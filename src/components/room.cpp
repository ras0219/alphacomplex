#include "components/room.hpp"
#include "city.hpp"
#include "components/furniture.hpp"

void Room::init() {
  city->add_room(this);
  auto v = city->find_furniture(x, y, w, h);
  furniture = set_t(v.begin(), v.end());
  for (auto f : furniture)
    f->rooms.insert(this);
}

void Room::deinit() {
  for (auto f : furniture)
    f->rooms.erase(this);
  furniture.clear();
  city->del_room(this);
}
