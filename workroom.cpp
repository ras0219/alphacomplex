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

const char* FetchJobStep1::RAWNAME = "fetch1";
const char* FetchJobStep2::RAWNAME = "fetch2";

int FetchJobStep1::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Fetch docs at %d, %d", x, y);
}
int FetchJobStep2::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Return docs to %d, %d", x, y);
}

FetchJobStep1* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p) {
  return new FetchJobStep1(x1, y1, new FetchJobStep2(x2, y2, p));
}
