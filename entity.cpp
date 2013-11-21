#include "defs.hpp"
#include "entity.hpp"
#include "city.hpp"
#include "pathfind.hpp"

Entity* Entity::GLOB_ENTLIST = nullptr;

void Dwarf::update() {
  for (uint i = 0; i < offs.size(); ++i) {
    int x2 = x + offs[i].first;
    int y2 = y + offs[i].second;
    if (city.tile(x2, y2).walkable()) {
      remove();
      insert_after(city.ent(x2, y2));
      set_loc(x2, y2);
      return;
    }
  }
}

