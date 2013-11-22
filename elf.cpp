#include "elf.hpp"
#include "joblist.hpp"
#include "garbage.hpp"
#include "city.hpp"
#include "workroom.hpp"

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

      int x2;
      int y2;
      if (jobs.has_job()) {
        assert(job == nullptr);

        job = jobs.pop_job();
        job_it = active_jobs.add_job(job);

        if (job->rawname() == Garbage::RAWNAME) {
          path_to(job->as<GarbageJob>().g->x, job->as<GarbageJob>().g->y);
        } else if (job->rawname() == FetchJob::RAWNAME) {
          path_to(job->as<FetchJob>().x1, job->as<FetchJob>().y1);
        } else {
          assert(false);
        }
        state = WALKINGTOJOB;
        return;
      } else {
        x2 = rand() % city.getXSize();
        y2 = rand() % city.getYSize();

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
        if (job->rawname() == Garbage::RAWNAME) {
          path.clear();
          pathp = path.rbegin();
          state = CLEANING;
        } else if (job->rawname() == FetchJob::RAWNAME) {
          if (job->as<FetchJob>().part == 0) {
            job->as<FetchJob>().part = 1;
            path_to(job->as<FetchJob>().x2, job->as<FetchJob>().y2);
          } else {
            job->as<FetchJob>().parent->complete_job();

            path.clear();
            pathp = path.rbegin();

            active_jobs.remove_job(job_it);
            delete job;
            job = nullptr;
            state = CONFUSED;
          }
        }
      }
    }
    return;
  case CLEANING:
    if (energy >= 10) {
      energy = 0;

      assert(job != nullptr);

      Entity* e = city.ent(x, y);
      while (e != nullptr) {
        //cout << e->rawname() << endl;
        if (e == job->as<GarbageJob>().g) {
          delete e;

          active_jobs.remove_job(job_it);
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
  }
}
