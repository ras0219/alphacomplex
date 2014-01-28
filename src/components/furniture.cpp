#include "components/furniture.hpp"
#include "components/room.hpp"
#include "city.hpp"
#include "components/position.hpp"
#include "entities/entity.hpp"

void Furniture::init() {
  auto pos = parent->assert_get<struct Position>();

  std::vector<struct Room*> rs = pos->city().find_rooms(pos->x(), pos->y());
  rooms = set_t(rs.begin(), rs.end());

  for (auto r : rooms)
    r->furniture.insert(this);
}

void Furniture::deinit() {
  for (auto r : rooms)
    r->furniture.erase(this);
  rooms.clear();
}
