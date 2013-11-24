#pragma once

#include "component.hpp"
#include "job.hpp"

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

struct JobList {
  typedef list<Job*>::iterator iterator;

  list<Job*> jlist;

  JobList() {}

  iterator add_job(Job*);
  void remove_job(iterator);
  Job* pop_job();

  inline bool has_job() { return jlist.size() > 0; }
};

struct JobListing : Component {
  JobList* jlist;
  int x, y;
  string title;

  JobListing(int x_, int y_, JobList* l, const char* t)
    : jlist(l), x(x_), y(y_), title(t) { }

  virtual void render(Graphics& g);
};

extern JobList jobs;
extern JobList active_jobs;
