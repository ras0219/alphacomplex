#include "components/furniture.hpp"
#include "components/room.hpp"
#include "city.hpp"
#include "components/position.hpp"
#include "entities/entity.hpp"

void Furniture::init() {
  pos.city->add_furniture(this);
}

void Furniture::deinit() {
  pos.city->del_furniture(this);
}
