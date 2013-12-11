#include "workroom.hpp"
#include "city.hpp"
#include "job.hpp"
#include "citizen.hpp"
#include "windows.hpp"
#include "callbackai.hpp"
#include "sequenceai.hpp"
#include "pathai.hpp"

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
