#pragma once

#include "component.hpp"

#include <list>
#include <string>

using namespace std;

// Job list requirements:
//
// Fast insert jobs
// Fast reserve job (given ptr to job)
// Fast release job
// Fast remove job
// Fast retrieval of specific job type

struct Job {
  virtual const char* rawname() const = 0;

  virtual int description(char* buf, size_t n) const = 0;

  virtual ~Job() { }

  virtual Job* complete_step() { return nullptr; }

  template<class T>
  T& as() { return (T&)(*this); }
  template<class T>
  const T& as() const { return (const T&)(*this); }
};

struct JobStep : Job {
  JobStep(Job* j) : next_step(j) { }

  virtual ~JobStep() {
    delete next_step;
  }

  virtual Job* complete_step();

  Job* next_step;
};

struct JobList {
  typedef list<Job*>::iterator iterator;

  list<Job*> jlist;

  JobList() {}

  iterator add_job(Job*);
  void remove_job(iterator);
  Job* pop_job();

  inline bool has_job() { return jlist.size() > 0; }
};

extern JobList jobs;
extern JobList active_jobs;

struct JobListing : Component {
  JobList* jlist;
  int x, y;
  string title;

  JobListing(int x_, int y_, JobList* l, const char* t)
    : jlist(l), x(x_), y(y_), title(t) { }

  virtual void render(Graphics& g);
};
