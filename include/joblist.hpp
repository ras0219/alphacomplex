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
  Job* find_job(clearance c);

  list_t jlist;
};

struct JobListing : StaticWidget<JobListing> {
  JobList* jlist;
  std::string title;

  JobListing(JobList* l, const char* t) : jlist(l), title(t) { }

  void render(Graphics& g, render_box const& pos);
};

extern JobList jobs;
