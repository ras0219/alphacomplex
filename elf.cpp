#include "elf.hpp"
#include "joblist.hpp"
#include "garbage.hpp"
#include "city.hpp"

const char* Elf::RAWNAME = "elf";

void Elf::render(Graphics& g) const {
  //cerr << "rendering elf @ " << x << "," << y << endl;
  if (state == CONFUSED)
    g.putChar(x, y, '?');
  else
    g.putChar(x, y, pic);
}

void Elf::update() {
  ++energy;
  switch (state) {
  case CONFUSED:
    if (energy >= 20) {
      energy = 0;

      int x2;
      int y2;
      if (jobs.has_job()) {
        assert(job == nullptr);
        job = jobs.pop_job();
        active_jobs.add_job(job);
        assert(job->rawname() == Garbage::RAWNAME);
        x2 = job->as<GarbageJob>().x;
        y2 = job->as<GarbageJob>().y;
        assert(city.tile(x2, y2).walkable());
        path = pathfind(city, x, y, x2, y2);
        pathp = path.rbegin();
        state = WALKINGTOJOB;
        return;
      }
      x2 = rand() % 12;
      y2 = rand() % 12;

      if (city.tile(x2, y2).walkable() && (x != x2 || y != y2)) {
        path = pathfind(city, x, y, x2, y2);
        pathp = path.rbegin();
        //cerr << x2 << ", " << y2 << endl;
        state = WANDERING;
        return;
      }
    }
    return;
  case WALKINGTOJOB:
    if (energy >= 3 && path.size() > 0) {
      energy = 0;

      assert(city.tile(pathp->first, pathp->second).walkable());

      remove();
      insert_after(city.ent(pathp->first, pathp->second));
      set_loc(pathp->first, pathp->second);

      ++pathp;
      if (pathp == path.rend()) {
        path.clear();
        assert(job != nullptr);
        state = CLEANING;
      }
    }
    return;
  case CLEANING:
    if (energy >= 10) {
      energy = 0;

      Entity* e = city.ent(x, y);
      while (e != nullptr) {
        //cout << e->rawname() << endl;
        if (e->rawname() == Garbage::RAWNAME) {
          delete e;
          active_jobs.remove_job(job);
          delete job;
          job = nullptr;
          state = CONFUSED;
          return;
        }
        e = e->next;
      }
      assert(false /* no garbage? */);
    }
    return;
  case WANDERING:
    if (energy >= 5 && path.size() > 0) {
      energy = 0;

      assert(city.tile(pathp->first, pathp->second).walkable());

      remove();
      insert_after(city.ent(pathp->first, pathp->second));
      set_loc(pathp->first, pathp->second);

      ++pathp;
      if (pathp == path.rend()) {
        path.clear();
        state = CONFUSED;
      }
    }
    return;
  }
}
