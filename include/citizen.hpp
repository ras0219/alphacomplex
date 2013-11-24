#pragma once

#include "entity.hpp"

#include <list>

struct Citizen : SmartEntity {
  Citizen(int x_, int y_, char pic_ = 'E')
    : SmartEntity(x_, y_),
      pic(pic_),
      state(CONFUSED),
      job(nullptr) { }

  char pic;
  int energy = 0;
  int clean_supplies = 0;

  enum StateMachine {
    CONFUSED,
    WALKINGTOJOB,
    WANDERING,
    ACTIVITY
  } state;

  struct Job* job;
  list<struct Job*>::iterator job_it;

  vector<point> path;
  vector<point>::reverse_iterator pathp;

  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }

  virtual void render(Graphics& g) const;

  virtual void update();

  void path_to(int, int);
};
