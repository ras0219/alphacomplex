#pragma once

#include "defs.hpp"

#include <cassert>
#include <list>

struct Job {
  virtual const char* rawname() const = 0;
  virtual int description(char* buf, size_t n) const = 0;
  virtual Department::Mask department() = 0;
  virtual Security::Mask security() = 0;
  virtual ~Job() { }

  virtual void assign_task(struct Citizen*) = 0;
  virtual bool complete_walk(struct Citizen*) = 0;
  virtual bool complete_activity(struct Citizen*) = 0;


  template<class T>
  T& as() { return (T&)(*this); }
  template<class T>
  const T& as() const { return (const T&)(*this); }
};

struct MultiJob : Job {
  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }
  virtual int description(char*, size_t) const;
  virtual Department::Mask department();
  virtual Security::Mask security();
  virtual ~MultiJob() { for (auto j : subjobs) delete j; }

  MultiJob() {}
  MultiJob(const initializer_list<Job*>& il) : subjobs(il) { }

  virtual void assign_task(struct Citizen*);
  virtual bool complete_walk(struct Citizen*);
  virtual bool complete_activity(struct Citizen*);

  list<Job*> subjobs;
};

#include "citizen.hpp"

template<class Derived>
struct WalkToJob : Job {
  virtual const char* rawname() const { return Derived::RAWNAME; }
  WalkToJob(int x_, int y_) : x(x_), y(y_) { }

  virtual void assign_task(Citizen* e) {
    e->path_to(x, y);
    e->state = Citizen::WALKINGTOJOB;
  }
  virtual bool complete_walk(Citizen* e) {
    return true;
  }
  virtual bool complete_activity(Citizen* e) { assert(false); return true; }

  int x, y;
};

template<class Derived>
struct ActivityJob : Job {
  virtual const char* rawname() const { return Derived::RAWNAME; }
  ActivityJob() { }

  virtual void assign_task(Citizen* e) {
    e->energy = -duration();
    e->state = Citizen::ACTIVITY;
  }
  virtual bool complete_walk(Citizen* e) { assert(false); return true; }
  virtual bool complete_activity(Citizen* e) { return true; }

  virtual int duration() = 0;
};
