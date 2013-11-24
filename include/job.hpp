#pragma once

#include "defs.hpp"

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

