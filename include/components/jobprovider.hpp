#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"
#include "components/ai/job.hpp"

#include <memory>
#include <vector>

struct JobProvider : ComponentCRTP<Component::JobProvider, JobProvider> {
    using joblist_t = std::vector<std::shared_ptr<Job>>;
    ~JobProvider();

    // Public
    joblist_t to_provide_jobs;

    // Private
    joblist_t provided_jobs;
};

struct JobProviderSystem : SubSystem<JobProviderSystem, JobProvider> {
  void update_item(Ent*, JobProvider*);
};
