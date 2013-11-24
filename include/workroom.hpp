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

struct FetchJobStep1 : WalkToJob<FetchJobStep1> {
  static const char* RAWNAME;
  virtual int description(char* buf, size_t n) const;
  
  FetchJobStep1(int x_, int y_)
    :  WalkToJob(x_, y_) { }
};

struct FetchJobStep2 : WalkToJob<FetchJobStep2> {
  static const char* RAWNAME;
  virtual int description(char* buf, size_t n) const;

  FetchJobStep2(int x_, int y_, WorkRoom* p)
    : WalkToJob(x_, y_), parent(p) { }

  virtual bool complete_walk(struct Elf*);

  WorkRoom* parent;
};

Job* make_fetch_job(int x1, int y1, int x2, int y2, WorkRoom* p);
