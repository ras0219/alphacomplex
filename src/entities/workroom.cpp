#include "entities/workroom.hpp"
#include "city.hpp"
#include "components/ai/job.hpp"
#include "entities/citizen.hpp"
#include "windows.hpp"
#include "components/ai/callbackai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/pathai.hpp"
#include "components/room.hpp"
#include "components/jobprovider.hpp"

#include <cstdlib>
#include <cstdio>

extern int influence;

std::shared_ptr<Job> make_fetch_job(int x1, int y1, int x2, int y2, Ent* p);

struct MakeWorkScript : AIScript {
  virtual int start(AI*) {
    return 100;
  }
  virtual int update(AI* ai) {
    Ent* room = ai->parent;
    JobProvider* jobprov = room->assert_get<JobProvider>();

    if (rand() % 20 > 0)
      return 100;

    Room* pos = room->assert_get<Room>();
    int x2 = pos->r.x;
    int y2 = pos->r.y;

    int x = 1, y = 1;

    jobprov->to_provide_jobs.emplace_back(make_fetch_job(x, y, x2, y2, room));

    return 100;
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
    static std::string desc;
};

std::string MakeWorkScript::desc = "Making work";

Ent* make_workroom(City& c, int x, int y, int w, int h) {
  Ent* r = new Ent;

  Room* room = new Room(Rect{ &c, x, y, w, h });
  r->add(room);

  r->add(new AI(make_shared<MakeWorkScript>()));
  r->add(new JobProvider);

  r->add(&AISystem::singleton());
  r->add(&JobProviderSystem::singleton());
  return r;
}

Ent* make_workroom(Rect r) {
  return make_workroom(*r.city, r.x, r.y, r.w, r.h);
}

std::shared_ptr<Job> make_fetch_job(int x1, int y1, int x2, int y2, Ent*) {
  std::shared_ptr<SequenceAI> script = make_shared<SequenceAI>();

  script->add_task(make_shared<PathAI>(point(x1, y1)));
  script->add_task(make_shared<PathAI>(point(x2, y2)));
  script->add_task(make_callbackai([=] (AI*) { ++influence; }));

  return make_shared<Job>(
    "Fetch Documents",
    clearance{ Security::ALL, Department::RESEARCH },
    script
    );
}
