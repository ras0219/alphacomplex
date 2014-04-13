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
#include "components/item.hpp"

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
  virtual const std::string& description() const override {
      return desc;
  }

  int request_job(AI* ai) {
    if (job) {
      job->complete();
      job = nullptr;
    }

    Clearance* c = ai->parent->assert_get<Clearance>();

    job = jobs.find_job(*c);
    if (job) {
      job->reserve();
      return ai->add_task( job->script(), 1 );
    } else {
      return 30;
    }
  }

  Job* job;
  static std::string desc;
};
std::string IdleAI::desc = "Idling";

ItemProperties citizen_properties = {
  "citizen",
  100
};

Ent* new_citizen(Point pos, Security::Mask sec) {
  Ent* e = new Ent;
  e->emplace<Position>(pos);
  e->emplace<Movable>(pos);
  e->emplace<AI>(std::make_shared<IdleAI>());
  e->emplace<Clearance>(clearance{ sec, Department::random_dept() });
  e->emplace<Renderable>(Security::mask_to_dcode(sec)[0]);
  e->add(random_citizenname());
  e->emplace<Needs>();
  e->emplace<Item>(citizen_properties);

  e->add(&AISystem::singleton());
  e->add(&smsystem);
  e->add(&NeedsSystem::singleton());
  return e;
}
