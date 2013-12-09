#include "workroom.hpp"
#include "city.hpp"
#include "job.hpp"
#include "citizen.hpp"
#include "windows.hpp"

#include <cstdlib>
#include <cstdio>

const char* WorkRoom::RAWNAME = "workroom";

extern int influence;

void WorkRoom::update() {
  if (current_work == nullptr) {
    ++work_counter;

    int work_countdown = 400 - work_counter;
    if (rand() % work_countdown == 0) {
      // Congratulations! Mandatory bonus work!
      current_work = make_fetch_job(1, 1, x, y, this);
      jobs.add_job(current_work);
      work_counter = 0;
    }
  }
}

void WorkRoom::complete_job() {
  current_work = nullptr;
  influence += 1;
}

template<class F>
struct CallbackAI : AIState {
  CallbackAI(F g) : f(g) { }

  virtual int start(Citizen* c) {
    f();
    return complete(c);
  }
  virtual int update(Citizen*) { assert(false); return -1; }

  F f;
};
template<class F>
CallbackAI<F>* make_cbai(F f) { return new CallbackAI<F>(f); }

struct FetchJob : Job {
  FetchJob(int x1_, int y1_, int x2_, int y2_, WorkRoom* r_)
    : x1(x1_), y1(y1_), x2(x2_), y2(y2_), r(r_) { }

  virtual int description(char* buf, size_t n) const;
  
  virtual Department::Mask department() { return Department::RESEARCH; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual AIState* get_script(Citizen* c) const {
    return new SequenceAI{
      c->path_script(x1,y1),
        c->path_script(x2, y2),
        make_cbai([=]() { r->complete_job(); })
    };
  }

  int x1, y1, x2, y2;
  WorkRoom* r;
};

int FetchJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Fetch docs at %d, %d", x1, y1);
}

Job* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p) {
  return new FetchJob(x1, y1, x2, y2, p);
}
