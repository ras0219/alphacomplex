#pragma once

#include "entity.hpp"
#include "subsystem.hpp"
#include "job.hpp"

#include <memory>
#include <vector>
using std::vector;
using std::shared_ptr;

struct JobProvider : AspectStatic<Aspect::JobProvider, JobProvider> {
  ~JobProvider();

  // Public
  vector<Job*> to_provide_jobs;

  // Private
  vector<shared_ptr<Job>> provided_jobs;
};

struct JobProviderSystem : SubSystem<JobProviderSystem, JobProvider> {
  void update_item(value_type& p);
};

extern JobProviderSystem jpsystem;
