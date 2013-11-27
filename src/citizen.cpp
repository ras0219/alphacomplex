#include "citizen.hpp"
#include "joblist.hpp"
#include "garbage.hpp"
#include "city.hpp"
#include "workroom.hpp"
#include "clock.hpp"

#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

const char* Citizen::RAWNAME = "citizen";

char Citizen::render() const {
  //cerr << "rendering elf @ " << x << "," << y << endl;
  if (state == IDLE && animtime % 20 < 10)
    return '?';
  else
    return Security::mask_to_dcode(sec)[0];
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

struct SecRevJobStep2 : ActivityJob<SecRevJobStep2> {
  SecRevJobStep2(Citizen* t)
    : target(t), complete(false) { }

  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "Reviewing");
  }

  virtual Department::Mask department() { return Department::INTERNAL_SECURITY; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual bool complete_activity(Citizen* c) {
    if (!complete) {
      c->energy = -20;
      return false;
    }

    char buf[20];
    target->description(buf,20);
    stringstream ss;
    ss << buf << " has undergone an IntSec Review by ";
    c->description(buf,20);
    ss << buf << '.';
    announce(ss.str());
    return true;
  }
  virtual int duration() { return 50; }

  Citizen* target;
  bool complete;

  static const char* RAWNAME;
};
const char* SecRevJobStep2::RAWNAME = "secrevstep2";

struct SecRevJobStep2B : ActivityJob<SecRevJobStep2B> {
  SecRevJobStep2B(bool* f) : flag(f) {}
  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "Undergoing Review");
  }

  virtual Department::Mask department() { return Department::INTERNAL_SECURITY; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual bool complete_activity(Citizen* c) {
    *flag = true;
    c->intsec_review_job = nullptr;
    return true;
  }
  virtual int duration() { return 50; }

  bool* flag;
  static const char* RAWNAME;
};
const char* SecRevJobStep2B::RAWNAME = "secrevstep2b";

struct SecRevJobStep1 : Job {
  SecRevJobStep1(Citizen* t)
    : target(t)
    {
      s2 = new SecRevJobStep2(t);
      s2b = new SecRevJobStep2B(&s2->complete);
    }

  virtual const char* rawname() const { return RAWNAME; }
  virtual int description(char* buf, size_t n) const {
    int d = snprintf(buf, n, "IntSec Review of ");
    return target->description(buf+d, n-d) + d;
  }

  virtual void assign_task(Citizen* c) {
    c->path_to(target->x, target->y);
    c->state = Citizen::WALKINGTOJOB;
  }
  virtual bool complete_walk(Citizen* c) {
    if (c->x != target->x or c->y != target->y) {
      c->energy = -(rand() % 10);
      c->state = Citizen::ACTIVITY;
      return false;
    }
    target->interrupt(s2b);
    return true;
  }
  virtual bool complete_activity(Citizen* c) {
    assign_task(c);
    return false;
  }

  virtual Department::Mask department() { return Department::INTERNAL_SECURITY; }
  virtual Security::Mask security() { return Security::ALL; }

  Citizen* target;

  SecRevJobStep2* s2;
  SecRevJobStep2B* s2b;

  static const char* RAWNAME;
};
const char* SecRevJobStep1::RAWNAME = "secrevstep1";

Job* make_security_review_job(Citizen* reviewee) {
  SecRevJobStep1* srjs1 = new SecRevJobStep1(reviewee);
  MultiJob* j  = new MultiJob{srjs1, srjs1->s2};
  return j;
}

void Citizen::set_job(Job* j) {
  assert(job == nullptr);
  job = j;
  job_it = active_jobs.add_job(job);
  job->assign_task(this);
}

void Citizen::update() {
  ++energy;
  if (intsec_review_job == nullptr && rand() % 2400 == 0) {
    // Time for a random intsec review!
    // Approx every minute (60 secs * 20 fps * 2 = 2400)
    intsec_review_job = make_security_review_job(this);
    jobs.add_job(intsec_review_job);
  }
  switch (state) {
  case IDLE:
    if (energy >= 10) {
      energy = 0;

      if (job != nullptr) {
        char buf[100];
        description(buf, 100);
        cerr << buf << endl;
        cerr << job->rawname() << endl;
        while (job->rawname() == MultiJob::RAWNAME) {
          job = job->as<MultiJob>().subjobs.front();
          cerr << job->rawname() << endl;
        }
      }
      assert(job == nullptr);
      Job* j = jobs.pop_job(security(), department());
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
    assert(job != nullptr);
    if (energy >= 5) {
      energy = 0;
      if (pathp != path.rend()) {
        assert(city.tile(pathp->first, pathp->second).walkable());

        remove();
        insert_after(city.ent(pathp->first, pathp->second));
        set_loc(pathp->first, pathp->second);

        ++pathp;

        if (security() == Security::ORANGE) {
          Entity* e = city.ent(x,y)->next;
          while (e != nullptr) {
            if (e != this && e->rawname() == Citizen::RAWNAME) {
              Citizen& c = e->as<Citizen>();
              if (c.security() < Security::ORANGE) {
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
    assert(job != nullptr);
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
    if (job->rawname() == WanderingJob::RAWNAME) {
      finalize_job();
    } else {
      active_jobs.remove_job(job_it);
      suspended_jobs.push_back(job);
      job = nullptr;
    }
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
  assert(job != nullptr);
  active_jobs.remove_job(job_it);
  delete job;
  job = nullptr;
}

int Citizen::description(char* buf, size_t n) const {
  const char* dcode = Security::mask_to_dcode(security());
  return snprintf(buf, n, "[%s] %s-%05d", dcode, "Ziggy", ssn);
}

Security::Mask Citizen::security() const {
  return sec;
}
