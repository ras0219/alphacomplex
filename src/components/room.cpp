#include "components/room.hpp"
#include "city.hpp"
#include "components/furniture.hpp"

void Room::on_add() {
  r.city->add_room(this);
}

void Room::on_remove() {
  r.city->del_room(this);
}
