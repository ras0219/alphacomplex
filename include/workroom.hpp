#pragma once

#include "room.hpp"
#include "joblist.hpp"

struct WorkRoom : Room {
  static const char* RAWNAME;

  virtual const char* rawname() { return RAWNAME; }

  WorkRoom(int x_, int y_, int w_, int h_, char c)
    : Room(x_,y_,w_,h_),
      work_counter(0),
      current_work(nullptr) { }

  int work_counter;
  struct Job* current_work;

  void complete_job();
  virtual void update();
};

struct FetchJobStep1 : JobStep {
  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual int description(char* buf, size_t n) const;
  
  FetchJobStep1(int x_, int y_, Job* part2)
    :  JobStep(part2), x(x_), y(y_) { }

  int x, y;
};

struct FetchJobStep2 : Job {
  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual int description(char* buf, size_t n) const;

  FetchJobStep2(int x_, int y_, WorkRoom* p)
    : x(x_), y(y_), parent(p) { }

  int x, y;

  WorkRoom* parent;
};

FetchJobStep1* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p);
