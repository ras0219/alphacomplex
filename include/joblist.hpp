#pragma once

#include "defs.hpp"
#include "component.hpp"
#include "clearance.hpp"

#include <list>
#include <string>

using namespace std;

struct Job;

struct JobList {
  iterator add_job(Job*);
  void remove_jobs();
  Job* find_job(Clearance c);

  typedef list<Job*>::iterator iterator;
  list<Job*> jlist;
};

struct JobListing : Component {
  JobList* jlist;
  int y;
  string title;

  JobListing(int y_, JobList* l, const char* t)
    : jlist(l), y(y_), title(t) { }

  virtual void render(Graphics& g);
};

extern JobList jobs;
