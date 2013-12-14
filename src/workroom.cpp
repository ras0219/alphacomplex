#include "workroom.hpp"
#include "city.hpp"
#include "job.hpp"
#include "citizen.hpp"
#include "windows.hpp"
#include "callbackai.hpp"
#include "sequenceai.hpp"
#include "pathai.hpp"
#include "room.hpp"
#include "jobprovider.hpp"

#include <cstdlib>
#include <cstdio>

extern int influence;

Job* make_fetch_job(int x1, int y1, int x2, int y2, Ent* p);

struct MakeWorkScript : AIScript {
  virtual int start(AI* ai) {
    return 100;
  }
  virtual int update(AI* ai) {
    if (rand() % 20 > 0)
      return 100;

    Ent* room = ai->parent;
    PositionComp* pos = room->get<PositionComp>();
    JobProvider* jobprov = room->get<JobProvider>();
    int x2 = pos->x();
    int y2 = pos->y();

    int x = 1, y = 1;

    jobprov->to_provide_jobs.emplace_back(make_fetch_job(x, y, x2, y2, room));

    return 100;
  }
};

Ent* make_workroom(City* c, int x, int y, int w, int h) {
  Ent* r = new Ent;
  
  Room* room = new Room(c, x, y, w, h);
  r->add(room);
  room->init();

  AIScript* script = new MakeWorkScript();

  AI* ai = new AI(script);
  r->add(ai);
  return r;
}

Job* make_fetch_job(int x1, int y1, int x2, int y2, Ent* p) {
  AIScript* script = new SequenceAI({
    new PathAI(point(x1, y1)),
    new PathAI(point(x2, y2)),
    new_callbackai([=]() {
      ++influence;
    })
  });

  Job* r = new Job{
    "Fetch Documents",
    { Security::ALL, Department::RESEARCH },
    script
  };
  return r;
}
