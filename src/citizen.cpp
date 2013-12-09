#include "citizen.hpp"
#include "joblist.hpp"
#include "garbage.hpp"
#include "city.hpp"
#include "workroom.hpp"
#include "clock.hpp"
#include "ai.hpp"
#include "windows.hpp"

#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

vector<string> names = {
  "Alex",
  "Alice",
  "Ansi",
  "Arnold",
  "Ben",
  "Bob",
  "Bill",
  "Casie",
  "Cal",
  "David",
  "Dan",
  "John",
  "Jim",
  "James",
  "Zim"
};

vector<string> sectors = {
  "QNX",
  "ABC",
  "TRN",
  "NZB",
  "EST",
  "PST",
  "CST",
  "ZNM",
  "OOO",
  "OPS",
  "UPS",
  "QAZ",
  "SDF",
  "MLY",
  "TME",
  "GOX"
};

struct IdleAI : AIScript {
  IdleAI() : job(nullptr) { }

  virtual int start(AI* c) {
    return request_job(c);
  }
  virtual int update(AI* c) {
    return request_job(c);
  }

  int request_job(AI* ai) {
    if (job) {
      job->complete();
      job = nullptr;
    }

    assert(ai->parent->has(EComp::Clearance));
    Clearance* c = ai->parent->get<Clearance>();

    job = jobs.find_job(*c);
    if (job) {
      job->reserve();
      return ai->push_script( job->script() );
    } else {
      return 30;
    }
  }

  Job* job;
};

Ent* make_new_red(Position pos) {
  Ent* e = new Ent();
  e->pos = new PositionComp(pos);
  e->pos->set_parent(e);
  e->add(new AI(new IdleAI()));
  e->add(new ClearanceComp({Security::RED, Department::random_dept()}));
  e->add(new Renderable(Security::mask_to_dcode(Security::RED)[0]));
}

const char* Citizen::RAWNAME = "citizen";

char Citizen::render() const {
  // if (state == IDLE && animtime % 20 < 10)
  //   return '?';
  // else if (state == SLEEPING && animtime % 20 < 10)
  //   return 'Z';
  // else
    return Security::mask_to_dcode(sec)[0];
}

struct ActivityAtAI : AIState {
  ActivityAtAI(point d, int dur) : walking(false), duration(dur), dest(d) { }

  virtual int start(Citizen* c) {
    //assert(c->city.tile(x2, y2).walkable());
    if (c->x != dest.first || c->y != dest.second) {
      path = pathfind(c->city, c->x, c->y, dest.first, dest.second);
      pathp = path.rbegin();
      walking = true;
      return 5;
    }
    walking = false;
    return duration;
  }

  virtual int update(Citizen* c) {
    if (walking) {
      if (pathp != path.rend()) {
        assert(c->city.tile(pathp->first, pathp->second).walkable());
        c->set_loc(pathp->first, pathp->second);
        ++pathp;
      }

      if (pathp == path.rend()) {
        walking = false;
        return duration;
      }
      return 5;
    } else {
      return complete(c);
    }
  }

  bool walking;
  int duration;
  point dest;
  vector<point> path;
  vector<point>::reverse_iterator pathp;
};

struct JobAI : AIState {
  JobAI(Job* j) : job(j), child(nullptr) { }

  virtual int start(Citizen* c) {
    job->state = Job::RESERVED;
    child = job->get_script(c);
    return c->push_aistate(child);
  }
  virtual int update(Citizen* c) {
    assert(false);
    return -1;
  }
  virtual int resume(Citizen* c, AIState* s) {
    assert(s == child);
    child = nullptr;
    job->state = Job::COMPLETED;
    return complete(c);
  }

  Job* job;
  AIState* child;
};

int Citizen::push_aistate(AIState* ai) {
  // suspend old state
  aistate->suspend(this, ai);
  // link in current state
  ai->set_parent(aistate);
  aistate = ai;
  // dispatch call to start
  return ai->start(this);
}

AIState* Citizen::path_script(int x2, int y2) {
  assert(city.tile(x2, y2).walkable());

  return new PathAI({x2, y2});
}
AIState* Citizen::job_script(Job* j) {
  return new JobAI(j);
}
AIState* Citizen::activity_script(int duration) { 
  return new ActivityAI(duration);
}
AIState* Citizen::path_activity_script(int tx, int ty, int d) {
  return new ActivityAtAI({tx, ty}, d);
}

struct SleepAI : AIState {
  virtual int start(Citizen* c) {
    return 100;
  }
  virtual int resume(Citizen* c, AIState*) {
    return complete(c);
  }
  virtual int update(Citizen* c) {
    if (hour(gametime) < 8)
      return 100;
    else
      return complete(c);
  }
};

void Citizen::update() {
  ++energy;
  // if (intsec_review_job == nullptr && rand() % 2400 == 0) {
  //   // Time for a random intsec review!
  //   // Approx every minute (60 secs * 20 fps * 2 = 2400)
  //   intsec_review_job = make_security_review_job(this);
  //   jobs.add_job(intsec_review_job);
  // }
  if (energy >= 0)
    energy = -aistate->update(this);
}

int Citizen::description(char* buf, size_t n) const {
  if (security() == Security::INFRARED)
    return snprintf(buf, n, "%s-%s [%1d]", name.c_str(), sect.c_str(), ssn);
  const char* dcode = Security::mask_to_dcode(security());
  return snprintf(buf, n, "%s-%c-%s [%1d]", name.c_str(), *dcode, sect.c_str(), ssn);
}

Security::Mask Citizen::security() const {
  return sec;
}

Citizen::Citizen(int x_, int y_, Security::Mask s, City& c)
  : AIEntity(x_, y_, c),
    sec(s),
    fact(Faction::PLAYER),
    dept(Department::random_dept()),
    aistate(new IdleAI()),
    //intsec_review_job(nullptr),
    ssn(rand() % 10000)
{
  name = names[rand() % names.size()];
  sect = sectors[rand() % sectors.size()];
  
  skills = Skill::random_skills(sec);
}

