#include "components/furniture.hpp"
#include "components/room.hpp"
#include "city.hpp"
#include "components/position.hpp"

void Furniture::init() {
  PositionComp* pos = parent->assert_get<PositionComp>();

  vector<struct Room*> rs = pos->city().find_rooms(pos->x(), pos->y());
  rooms = set_t(rs.begin(), rs.end());

  for (auto r : rooms)
    r->furniture.insert(this);
}

void Furniture::deinit() {
  for (auto r : rooms)
    r->furniture.erase(this);
  rooms.clear();
}
