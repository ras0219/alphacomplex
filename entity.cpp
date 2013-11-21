#include "defs.hpp"
#include "entity.hpp"
#include "city.hpp"
#include "pathfind.hpp"

Entity* Entity::GLOB_ENTLIST = nullptr;

void Dwarf::update() {
  ++energy;
  if (energy >= 5) {
    energy -= 5;

    for (uint j = 0; j < offs.size(); ++j) {
      int i = (j + facing - 1) % offs.size();

      int x2 = x + offs[i].first;
      int y2 = y + offs[i].second;

      if (city.tile(x2, y2).walkable()) {
        remove();
        insert_after(city.ent(x2, y2));
        set_loc(x2, y2);
        facing = (Direction)i;
        return;
      }
    }
  }
}

void Elf::update() {
  ++energy;
  if (energy >= 3 && path.size() > 0) {
    energy -= 3;

    assert(city.tile(pathp->first, pathp->second).walkable());

    remove();
    insert_after(city.ent(pathp->first, pathp->second));
    set_loc(pathp->first, pathp->second);

    ++pathp;
    if (pathp == path.rend())
      path.clear();
    return;
  } else if (energy >= 20 && path.size() == 0) {
    energy -= 20;

    int x2 = rand() % 12;
    int y2 = rand() % 12;

    if (city.tile(x2, y2).walkable()) {
      path = pathfind(city, x, y, x2, y2);
      pathp = path.rbegin();
      cerr << x2 << ", " << y2 << endl;
    }
  }
}
