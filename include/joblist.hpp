#pragma once

#include "defs.hpp"
#include "component.hpp"

#include <list>
#include <string>

using namespace std;

struct Job;

struct JobList {
  typedef list<Job*>::iterator iterator;

  list<Job*> jlist;

  JobList() {}

  iterator add_job(Job*);
  void remove_job(iterator);
  Job* pop_job(Security::Mask = Security::RED,
               Department::Mask = Department::ALL);
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
extern JobList active_jobs;
