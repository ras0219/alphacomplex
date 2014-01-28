#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"
#include "components/ai/job.hpp"

#include <memory>
#include <vector>
using std::vector;
using std::shared_ptr;

struct JobProvider : ComponentCRTP<Component::JobProvider, JobProvider> {
  ~JobProvider();

  // Public
  vector<shared_ptr<Job>> to_provide_jobs;

  // Private
  vector<shared_ptr<Job>> provided_jobs;
};

struct JobProviderSystem : SubSystem<JobProviderSystem, JobProvider> {
  void update_item(Ent*, JobProvider*);
};

extern JobProviderSystem jpsystem;
