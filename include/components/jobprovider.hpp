#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"
#include "components/ai/job.hpp"

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
  void update(value_type& p);
};

extern JobProviderSystem jpsystem;
