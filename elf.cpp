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

        if (clean_supplies == 0 && job->rawname() == Garbage::RAWNAME) {
          // Must fetch cleaning supplies
          Room* cleaning = city.find_room(CleaningRoom::RAWNAME);
          if (cleaning == nullptr) {
            // Magic some supplies
            clean_supplies = 3;
          } else {
            job = new SupplyJob(cleaning->x, cleaning->y, job);
          }
        }

        job_it = active_jobs.add_job(job);

        if (job->rawname() == Garbage::RAWNAME) {
          path_to(job->as<GarbageJob>().g->x, job->as<GarbageJob>().g->y);
        } else if (job->rawname() == SupplyJob::RAWNAME) {
          path_to(job->as<SupplyJob>().x, job->as<SupplyJob>().y);
        } else if (job->rawname() == FetchJobStep1::RAWNAME) {
          path_to(job->as<FetchJobStep1>().x, job->as<FetchJobStep1>().y);
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
        } else if (job->rawname() == SupplyJob::RAWNAME) {
          clean_supplies = 3;

          complete_job_step();

          path_to(job->as<GarbageJob>().g->x, job->as<GarbageJob>().g->y);
        } else if (job->rawname() == FetchJobStep1::RAWNAME) {
          complete_job_step();

          path_to(job->as<FetchJobStep2>().x, job->as<FetchJobStep2>().y);
        } else if (job->rawname() == FetchJobStep2::RAWNAME) {
          job->as<FetchJobStep2>().parent->complete_job();

          path.clear();
          pathp = path.rbegin();

          active_jobs.remove_job(job_it);
          delete job;
          job = nullptr;
          state = CONFUSED;
        }
      }
    }
    return;
  case CLEANING:
    if (energy >= 10) {
      energy = 0;

      assert(job != nullptr);
      assert(job->rawname() == Garbage::RAWNAME);
      assert(clean_supplies > 0);

      Entity* e = city.ent(x, y);
      while (e != nullptr) {
        //cout << e->rawname() << endl;
        if (e == job->as<GarbageJob>().g) {
          --clean_supplies;

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

void Elf::complete_job_step() {
  Job* j = job->as<JobStep>().complete_step();

  active_jobs.remove_job(job_it);
  delete job;
  job = j;
  job_it = active_jobs.add_job(job);
}
