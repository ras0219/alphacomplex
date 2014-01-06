#pragma once

#include "defs.hpp"
#include "views/widget.hpp"
#include "components/clearance.hpp"

#include <list>
#include <string>
#include <memory>

using namespace std;

struct Job;

struct JobList {
  using list_t = list<shared_ptr<Job>>;
  using iterator = list_t::iterator;

  iterator add_job(Job* j);
  iterator add_job(const shared_ptr<Job>& j);
  void remove_jobs();
  Job* find_job(Clearance c);

  list_t jlist;
};

struct JobListing : Widget {
  JobList* jlist;
  int y;
  string title;

  JobListing(int y_, JobList* l, const char* t)
    : jlist(l), y(y_), title(t) { }

  virtual void render(Graphics& g);
};

extern JobList jobs;
