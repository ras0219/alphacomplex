#include "workroom.hpp"
#include "city.hpp"
#include "job.hpp"
#include "citizen.hpp"
#include "windows.hpp"
#include "callbackai.hpp"
#include "sequenceai.hpp"
#include "pathai.hpp"
#include "room.hpp"

#include <cstdlib>
#include <cstdio>

extern int influence;

Job* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p);

Ent* make_workroom(City* c, int x, int y, int w, int h) {
  Ent* r = new Ent;
  
  Room* room = new Room(c, x, y, w, h);
  r->add(room);
  room->init();

  return r;
}

Job* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p) {
  AIScript* script = new SequenceAI {
    new PathAI({x1,y1}),
    new PathAI({x2,y2}),
    new_callbackai([=]() { p->complete_job(); })
  };

  Job* r = new Job {
    "Fetch Documents",
    { Security::ALL, Department::RESEARCH },
    script
  };
  return r;
}
