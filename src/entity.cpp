#include "defs.hpp"
#include "entity.hpp"
#include "city.hpp"
#include "pathfind.hpp"
#include "joblist.hpp"

#include "garbage.hpp"

#include <algorithm>
#include <vector>

list<AIEntity*> AIEntity::ai_list;

void Entity::insert_after(Entity* e) {
  next = e->next;
  prev = e;
  e->next = this;
  if (next != nullptr)
    next->prev = this;
}

void Entity::remove() {
  if (next != nullptr)
    next->prev = prev;
  if (prev != nullptr)
    prev->next = next;
}



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
    jobs.add_job(make_garbage_job(g));
  }
}

char Dwarf::render() const { return pic; }
