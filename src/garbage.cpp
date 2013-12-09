#include "city.hpp"
#include "garbage.hpp"
#include "windows.hpp"
#include "citizen.hpp"

#include <cstdio>

const char* CleaningRoom::RAWNAME = "cleaningroom";

template<class T>
struct DeleteAI : AIState {
  DeleteAI(T* t) : ptr(t) { }

  virtual int start(Citizen* c) {
    delete ptr;
    return complete(c);
  }
  virtual int update(Citizen* c) {
    assert(false);
    return -1;
  }

  T* ptr;
};

struct GarbageJob : Job {
  GarbageJob(Garbage* g_) : g(g_) { }

  virtual int description(char* buf, size_t n) const;
  virtual Department::Mask department() { return Department::FACILITIES; }
  virtual Security::Mask security() {
    return (Security::Mask)(Security::INFRARED | Security::RED);
  }

  virtual AIState* get_script(Citizen* c) const {
    return new SequenceAI({ c->path_activity_script(g->x, g->y, 100), new DeleteAI<Garbage>(g) });
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

  Garbage* g;
};
int GarbageJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Clean Garbage @ %d, %d", g->x, g->y);
}

const char* Garbage::RAWNAME = "garbage";

// int CleaningJob::description(char* buf, size_t n) const {
//   return snprintf(buf, n, "Cleaning Garbage");
// }

// int SupplyJob::description(char* buf, size_t n) const {
//   return snprintf(buf, n, "Fetch Cleaning Supplies");
// }

Job* make_garbage_job(Garbage* g) {
  return new GarbageJob(g);
}
