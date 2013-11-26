#include "workroom.hpp"
#include "city.hpp"
#include "job.hpp"

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

struct FetchJobStep1 : WalkToJob<FetchJobStep1> {
  static const char* RAWNAME;
  virtual int description(char* buf, size_t n) const;
  
  virtual Department::Mask department() { return Department::RESEARCH; }
  virtual Security::Mask security() { return Security::ALL; }

  FetchJobStep1(int x_, int y_)
    :  WalkToJob(x_, y_) { }
};

struct FetchJobStep2 : WalkToJob<FetchJobStep2> {
  static const char* RAWNAME;
  virtual int description(char* buf, size_t n) const;

  virtual Department::Mask department() { return Department::RESEARCH; }
  virtual Security::Mask security() { return Security::ALL; }

  FetchJobStep2(int x_, int y_, WorkRoom* p)
    : WalkToJob(x_, y_), parent(p) { }

  virtual bool complete_walk(struct Citizen*);

  WorkRoom* parent;
};

const char* FetchJobStep1::RAWNAME = "fetch1";
const char* FetchJobStep2::RAWNAME = "fetch2";

int FetchJobStep1::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Fetch docs at %d, %d", x, y);
}
int FetchJobStep2::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Return docs to %d, %d", x, y);
}

bool FetchJobStep2::complete_walk(struct Citizen* e) {
  parent->complete_job();
  return true;
}

Job* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p) {
  return new MultiJob{ new FetchJobStep1(x1, y1), new FetchJobStep2(x2, y2, p) };
}
