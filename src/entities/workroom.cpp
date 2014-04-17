#include "entities/workroom.hpp"
#include "entities/citizen.hpp"
#include "job/job.hpp"
#include "job/jobprovider.hpp"
#include "components/ai/callbackai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/pathai.hpp"
#include "components/room.hpp"
#include "city.hpp"
#include "windows.hpp"
#include "point.hpp"

#include <cstdlib>
#include <cstdio>

extern int influence;

using namespace ai;
using namespace ecs;

std::shared_ptr<job::Job> make_fetch_job(int x1, int y1, int x2, int y2, ecs::Ent* p);

struct MakeWorkScript : AIScript {
  virtual int start(AI*) {
    return 100;
  }
  virtual int update(AI* ai) {
    Ent* room = ai->parent;
    auto jobprov = room->assert_get<job::JobProvider>();

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

Ent* make_workroom(const Rect& rec) {
    Ent* r = new Ent;

    r->emplace<Room>(rec);
    r->emplace<AI>(std::make_shared<MakeWorkScript>());
    r->emplace<job::JobProvider>();

    r->emplace_system<AISystem>();
    r->emplace_system<job::JobProviderSystem>();
    return r;
}

std::shared_ptr<job::Job> make_fetch_job(int x1, int y1, int x2, int y2, Ent*) {
  std::shared_ptr<SequenceAI> script = make_shared<SequenceAI>();

  script->add_task(make_shared<PathAI>(point(x1, y1)));
  script->add_task(make_shared<PathAI>(point(x2, y2)));
  script->add_task(make_callbackai([=] (AI*) { ++influence; }));

  return std::make_shared<job::Job>(
    "Fetch Documents",
    clearance{ Security::ALL, Department::RESEARCH },
    script
    );
}
