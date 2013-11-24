#include "elf.hpp"
#include "joblist.hpp"
#include "garbage.hpp"
#include "city.hpp"
#include "workroom.hpp"

#include <cassert>

const char* Elf::RAWNAME = "elf";

void Elf::render(Graphics& g) const {
  //cerr << "rendering elf @ " << x << "," << y << endl;
  if (state == CONFUSED)
    g.putChar(x, y, '?');
  else
    g.putChar(x, y, pic);
}

struct WanderingJob : Job {
  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual void assign_task(Elf*) { }
  virtual bool complete_walk(Elf*) { return true; }
  virtual bool complete_activity(Elf*) { assert(false); return true; }

  const char* msg;

  static const array<const char*, 4> msgs;

  WanderingJob(const char* m) : msg(m) { }
  WanderingJob() {
    msg = msgs[rand() % msgs.size()];
  }

  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "%s", msg);
  }
};

const array<const char*, 4> WanderingJob::msgs =
{{
    "Avoiding Narcolepsy",
    "Obtaining Trouble",
    "Avoiding Work",
    "Chasing Silence"
  }};

const char* WanderingJob::RAWNAME = "wandering";

void Elf::path_to(int x2, int y2) {
  assert(city.tile(x2, y2).walkable());
  path = pathfind(city, x, y, x2, y2);
  pathp = path.rbegin();
}

void Elf::update() {
  ++energy;
  switch (state) {
  case CONFUSED:
    if (energy >= 20) {
      energy = 0;

      if (jobs.has_job()) {
        assert(job == nullptr);

        job = jobs.pop_job();

        // if (clean_supplies == 0 && job->rawname() == Garbage::RAWNAME) {
        //   // Must fetch cleaning supplies
        //   Room* cleaning = city.find_room(CleaningRoom::RAWNAME);
        //   if (cleaning == nullptr) {
        //     // Magic some supplies
        //     clean_supplies = 3;
        //   } else {
        //     clean_supplies = 3;
        //     //job = new SupplyJob(cleaning->x, cleaning->y, job);
        //   }
        // }

        job_it = active_jobs.add_job(job);

        job->assign_task(this);
        return;
      } else {
        int x2 = rand() % city.getXSize();
        int y2 = rand() % city.getYSize();

        if (city.tile(x2, y2).walkable()) {
          path_to(x2, y2);

          job = new WanderingJob();
          job_it = active_jobs.add_job(job);
          state = WANDERING;
          return;
        }
      }
    }
    return;
  case WALKINGTOJOB:
    assert(job != nullptr);
    if (energy >= 2) {
      energy = 0;
      if (pathp != path.rend()) {
        assert(city.tile(pathp->first, pathp->second).walkable());
        
        remove();
        insert_after(city.ent(pathp->first, pathp->second));
        set_loc(pathp->first, pathp->second);
        
        ++pathp;
      }
      if (pathp == path.rend()) {
        if (job->complete_walk(this)) {
          active_jobs.remove_job(job_it);
          delete job;
            
          job = nullptr;
          state = CONFUSED;
          return;
        }
      }
    }
    return;
  case WANDERING:
    if (energy >= 5) {
      energy = 0;
      if (pathp != path.rend()) {
        assert(city.tile(pathp->first, pathp->second).walkable());

        remove();
        insert_after(city.ent(pathp->first, pathp->second));
        set_loc(pathp->first, pathp->second);

        ++pathp;
      }
      if (pathp == path.rend()) {
        path.clear();
        pathp = path.rbegin();
        
        active_jobs.remove_job(job_it);
        delete job;
        job = nullptr;
        state = CONFUSED;
      }
    }
    return;
  case ACTIVITY:
    if (energy >= 0) {
      energy = 0;
      if (job->complete_activity(this)) {
        active_jobs.remove_job(job_it);
        delete job;
            
        job = nullptr;
        state = CONFUSED;
        return;
      }
    }
  }
}
