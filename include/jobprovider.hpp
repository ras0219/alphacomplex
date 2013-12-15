#pragma once

#include "entity.hpp"
#include "subsystem.hpp"

#include <vector>
using std::vector;

struct Job;

struct JobProvider : AspectStatic<Aspect::JobProvider, JobProvider> {
  ~JobProvider();
  // Public
  vector<Job*> to_provide_jobs;

  // Private
  vector<Job*> provided_jobs;
};

struct JobProviderSystem : SubSystem<JobProviderSystem, JobProvider> {
  void update(value_type& p);
};

extern JobProviderSystem jpsystem;
