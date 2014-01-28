#include "components/room.hpp"
#include "city.hpp"
#include "components/furniture.hpp"

void Room::init() {
  r.city->add_room(this);
}

void Room::deinit() {
  r.city->del_room(this);
}
