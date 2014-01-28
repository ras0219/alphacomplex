#include "components/furniture.hpp"
#include "components/room.hpp"
#include "city.hpp"
#include "components/position.hpp"
#include "entities/entity.hpp"

void Furniture::on_add() {
  pos.city->add_furniture(this);
}

void Furniture::on_remove() {
  pos.city->del_furniture(this);
}
