#include "citizen.hpp"
#include "joblist.hpp"
#include "garbage.hpp"
#include "city.hpp"
#include "workroom.hpp"

#include <cassert>

const char* Citizen::RAWNAME = "citizen";

char Citizen::render() const {
  //cerr << "rendering elf @ " << x << "," << y << endl;
  if (state == IDLE)
    return '?';
  else
    return pic;
}

struct WanderingJob : WalkToJob<WanderingJob> {
  static const char* RAWNAME;

  const char* msg;
  static const array<const char*, 4> msgs;

  WanderingJob(int x_, int y_) : WalkToJob(x_, y_) {
    msg = msgs[rand() % msgs.size()];
  }

  virtual Department::Mask department() { return Department::ALL; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual void assign_task(Citizen* c) {
    this->WalkToJob<WanderingJob>::assign_task(c);
    c->state = Citizen::WANDERING;
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

void Citizen::path_to(int x2, int y2) {
  assert(city.tile(x2, y2).walkable());
  path = pathfind(city, x, y, x2, y2);
  pathp = path.rbegin();
}

struct GetToldOffJob : ActivityJob<GetToldOffJob> {
  static const char* RAWNAME;

  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "Getting told off");
  }

  virtual Department::Mask department() { return Department::ALL; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual int duration() { return 40; }
};
const char* GetToldOffJob::RAWNAME = "gettoldoffjob";

struct TellOffJob : ActivityJob<TellOffJob> {
  static const char* RAWNAME;

  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "Telling off a subordinate");
  }

  virtual Department::Mask department() { return Department::ALL; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual int duration() { return 40; }
};
const char* TellOffJob::RAWNAME = "telloffjob";

void Citizen::set_job(Job* j) {
  assert(job == nullptr);
  job = j;
  job_it = active_jobs.add_job(job);
  job->assign_task(this);
}

void Citizen::update() {
  ++energy;
  switch (state) {
  case IDLE:
    if (energy >= 10) {
      energy = 0;

      assert(job == nullptr);
      Job* j = jobs.pop_job();
      if (j) {
        return set_job(j);
      } else {
        int x2 = rand() % city.getXSize();
        int y2 = rand() % city.getYSize();

        if (city.tile(x2, y2).walkable()) {
          set_job(new WanderingJob(x2, y2));
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
          finalize_job();
            
          return resume();
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

        if (pic == 'O') {
          Entity* e = city.ent(x,y)->next;
          while (e != nullptr) {
            if (e != this && e->rawname() == Citizen::RAWNAME) {
              Citizen& c = e->as<Citizen>();
              if (c.pic != 'O') {
                // Oh boy, a subordinate!

                interrupt(new TellOffJob());
                c.interrupt(new GetToldOffJob());
                return;
              }
            }
            e = e->next;
          }
        }
      }
      if (pathp == path.rend()) {
        path.clear();
        pathp = path.rbegin();

        finalize_job();
        return resume();
      }
    }
    return;
  case ACTIVITY:
    if (energy >= 0) {
      energy = 0;
      if (job->complete_activity(this)) {
        finalize_job();
        return resume();
      }
    }
  }
}

void Citizen::interrupt(Job* j) {
  if (job != nullptr) {
    active_jobs.remove_job(job_it);
    suspended_jobs.push_back(job);
    job = nullptr;
  }

  return set_job(j);
}

void Citizen::resume() {
  assert(job == nullptr);
  if (suspended_jobs.empty()) {
    state = IDLE;
    return;
  }
  Job* j = suspended_jobs.back();
  suspended_jobs.pop_back();
  set_job(j);
}

void Citizen::finalize_job() {
  active_jobs.remove_job(job_it);
  delete job;
  job = nullptr;
}

int Citizen::description(char* buf, size_t n) const {
  const char* dcode = Security::mask_to_dcode(pic == 'O' ? Security::ORANGE : Security::RED);
  return snprintf(buf, n, "[%s] %s-01", dcode, "Ziggy");
}
