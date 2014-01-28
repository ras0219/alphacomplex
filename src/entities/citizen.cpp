#include "entities/citizen.hpp"
#include "joblist.hpp"
#include "city.hpp"
#include "clock.hpp"
#include "components/ai/ai.hpp"
#include "components/ai/job.hpp"
#include "components/ai/needsai.hpp"
#include "components/renderable.hpp"
#include "components/citizenname.hpp"
#include "components/movable.hpp"
#include "components/position.hpp"

#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cassert>

string get_full_name(Ent* e) {
  CitizenName* cn = e->assert_get<CitizenName>();
  Clearance* cc = e->assert_get<Clearance>();

  stringstream ss;
  ss << cn->name;

  if (cc->security() != Security::INFRARED)
    ss << '-' << Security::mask_to_dcode(cc->security())[0];

  ss << '-' << cn->sector;

  return ss.str();
}

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

    Clearance* c = ai->parent->assert_get<Clearance>();

    job = jobs.find_job(c->clearance());
    if (job) {
      job->reserve();
      return ai->push_script( job->script() );
    } else {
      return 30;
    }
  }

  Job* job;
};

Ent* new_citizen(Point pos, Security::Mask sec) {
  Ent* e = new Ent;
  e->add(new Position(pos));
  e->add(new Movable(pos));
  e->add(new AI(make_shared<IdleAI>()));
  e->add(new Clearance(clearance{ sec, Department::random_dept() }));
  e->add(new Renderable(Security::mask_to_dcode(sec)[0]));
  e->add(random_citizenname());
  e->add(new Needs());

  e->add(&aisystem);
  e->add(&smsystem);
  e->add(&needssystem);
  return e;
}
