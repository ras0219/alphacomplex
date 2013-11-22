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

struct FetchJob : Job {
  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual int description(char* buf, size_t n) const;

  FetchJob(int x1_, int y1_, int x2_, int y2_, WorkRoom* p)
    : part(0), x1(x1_), y1(y1_), x2(x2_), y2(y2_), parent(p)
    { }

  int part;

  int x1, y1;
  int x2, y2;

  WorkRoom* parent;
};
