#include "furniture.hpp"
#include "room.hpp"
#include "city.hpp"

void Furniture::init() {
  PositionComp* pos = parent->assert_get<PositionComp>();

  vector<Room*> rs = pos->city()->find_rooms(pos->x(), pos->y());
  rooms = set_t(rs.begin(), rs.end());

  for (auto r : rooms)
    r->furniture.insert(this);
}

void Furniture::deinit() {
  for (auto r : rooms)
    r->furniture.erase(this);
  rooms.clear();
}
