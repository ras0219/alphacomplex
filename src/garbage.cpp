#include "garbage.hpp"

const char* CleaningRoom::RAWNAME = "cleaningroom";

struct CleaningJob : ActivityJob<CleaningJob> {
  CleaningJob(Garbage* g_) : g(g_) { }

  virtual int description(char* buf, size_t n) const;
  virtual int duration() { return 20; }

  virtual bool complete_activity(Elf*) {
    delete g;
    return true;
  }

  static const char* RAWNAME;
  Garbage* g;
};

struct GarbageJob : WalkToJob<GarbageJob> {
  GarbageJob(Garbage* g_)
    : WalkToJob(g_->x, g_->y), g(g_) { }

  virtual int description(char* buf, size_t n) const;

  static const char* RAWNAME;
  Garbage* g;
};

struct SupplyJob : WalkToJob<SupplyJob> {
  SupplyJob(int x_, int y_)
    : WalkToJob(x_, y_) { }

  virtual int description(char* buf, size_t n) const;

  static const char* RAWNAME;
  Job* g;
  int x, y;
};

const char* Garbage::RAWNAME = "garbage";

const char* SupplyJob::RAWNAME = "supplyjob";
const char* GarbageJob::RAWNAME = "garbagejob";
const char* CleaningJob::RAWNAME = "cleaningjob";

int GarbageJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Clean Garbage @ %d, %d", g->x, g->y);
}

int CleaningJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Cleaning Garbage");
}

int SupplyJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Fetch Cleaning Supplies");
}

Job* make_garbage_job(Garbage* g) {
  return new MultiJob{new GarbageJob(g), new CleaningJob(g)};
}
