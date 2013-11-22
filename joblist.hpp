#pragma once

#include "component.hpp"

// Job list requirements:
//
// Fast insert jobs
// Fast reserve job (given ptr to job)
// Fast release job
// Fast remove job
// Fast retrieval of specific job type

struct Job {
  virtual const char* rawname() const = 0;

  Job* next;
  Job* prev;

  virtual ~Job() {}

  template<class T>
  T& as() { return (T&)(*this); }
  template<class T>
  const T& as() const { return (const T&)(*this); }
};

struct JobList {
  Job* head;

  JobList() : head(nullptr) {}

  void add_job(Job*);
  void remove_job(Job*);

  Job* pop_job();

  inline bool has_job() { return head != nullptr; }
};

extern JobList jobs;

struct JobListing : Component {
  JobList* jlist;
  int x, y;

  JobListing(int x_, int y_, JobList* l) : jlist(l), x(x_), y(y_) { }

  virtual void render(Graphics& g);
};
