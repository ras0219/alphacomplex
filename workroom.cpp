#include "workroom.hpp"
#include "city.hpp"

const char* WorkRoom::RAWNAME = "workroom";

extern int influence;

void WorkRoom::update() {
  if (current_work == nullptr) {
    ++work_counter;

    int work_countdown = 400 - work_counter;
    if (rand() % work_countdown == 0) {
      // Congratulations! Mandatory bonus work!
      current_work = new FetchJob(1, 1, x, y, this);
      jobs.add_job(current_work);
      work_counter = 0;
    }
  }
}

void WorkRoom::complete_job() {
  current_work = nullptr;
  influence += 1;
}

const char* FetchJob::RAWNAME = "fetch";

int FetchJob::description(char* buf, size_t n) const {
  if (part == 0)
    return snprintf(buf, n, "Fetch docs @ %d, %d", x1, y1);
  if (part == 1)
    return snprintf(buf, n, "Return docs @ %d, %d", x2, y2);
  assert(part >= 0 && part < 2);
  return -1;
}
