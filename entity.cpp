#include "defs.hpp"
#include "entity.hpp"
#include "city.hpp"
#include "pathfind.hpp"
#include "joblist.hpp"

#include "garbage.hpp"

#include <algorithm>
#include <vector>

JobList garbage_list;

Entity* Entity::GLOB_ENTLIST = nullptr;

const char* Dwarf::RAWNAME = "dwarf";

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

  if (rand() % 100 == 0) {
    Garbage* g = new Garbage(x, y);
    g->insert_after(this);
    jobs.add_job(new GarbageJob(x, y));
  }
}

