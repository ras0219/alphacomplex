#include "city.hpp"
#include "garbage.hpp"
#include "windows.hpp"
#include "citizen.hpp"
#include "deleteai.hpp"
#include "renderable.hpp"
#include "sequenceai.hpp"
#include "pathai.hpp"
#include "activityai.hpp"

#include <cstdio>

// const char* CleaningRoom::RAWNAME = "cleaningroom";

Ent* make_garbage(Position pos) {
  Ent* e = new Ent;
  e->add(new PositionComp(pos));
  e->add(new Renderable(';'));
  return e;
}


Job* make_garbage_job(Ent* e) {
  PositionComp* pos = e->assert_get<PositionComp>();

  Clearance c = {
    (Security::Mask)(Security::INFRARED |
                     Security::RED),
    Department::FACILITIES
  };

  AIScript* ais = new SequenceAI
    {
      new PathAI(pos->as_point()),
      new ActivityAI(100),
      new_deleteai(e)
    };
  return new Job("Clean Garbage", c, ais);
}
  // if (e->clean_supplies > 0) {
  //   --e->clean_supplies;
  //   return WalkToJob<GarbageJob>::assign_task(e);
  // } else {
  //   Room* cleaning = g->city.find_room(CleaningRoom::RAWNAME);
  //   if (cleaning == nullptr)
  //     return WalkToJob<GarbageJob>::assign_task(e);
      
  //   e->job->as<MultiJob>().subjobs.push_front(new SupplyJob(cleaning->x, cleaning->y));
  //   e->job->assign_task(e);
  // }

// int CleaningJob::description(char* buf, size_t n) const {
//   return snprintf(buf, n, "Cleaning Garbage");
// }

// int SupplyJob::description(char* buf, size_t n) const {
//   return snprintf(buf, n, "Fetch Cleaning Supplies");
// }

