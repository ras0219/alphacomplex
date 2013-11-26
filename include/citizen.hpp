#pragma once

#include "entity.hpp"
#include "defs.hpp"

#include <list>

struct Citizen : AIEntity {
  Citizen(int x_, int y_, char pic_ = 'R')
    : AIEntity(x_, y_),
      pic(pic_),
      state(IDLE),
      job(nullptr),
      dept(Department::ALL)
    { }

  virtual const char* rawname() const { return RAWNAME; }
  virtual int description(char* buf, size_t n) const;

  virtual char render() const;
  virtual void update();

  Security::Mask security() const;
  inline Department::Mask department() const { return dept; }

  void interrupt(struct Job*);
  void resume();
  void finalize_job();
  void path_to(int, int);
  void set_job(struct Job*);


  char pic;
  int energy = 0;
  int clean_supplies = 0;

  enum StateMachine {
    IDLE,
    WALKINGTOJOB,
    WANDERING,
    ACTIVITY
  } state;

  struct Job* job;
  list<struct Job*>::iterator job_it;
  vector<Job*> suspended_jobs;

  vector<point> path;
  vector<point>::reverse_iterator pathp;

  Department::Mask dept;

  static const char* RAWNAME;
};
