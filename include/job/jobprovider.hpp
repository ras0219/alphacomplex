#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"
#include "job.hpp"

#include <memory>
#include <vector>

namespace job {

    struct JobProvider : ecs::ComponentCRTP<ecs::Component::JobProvider, JobProvider> {
        using joblist_t = std::vector<std::shared_ptr<Job>>;
        ~JobProvider();

        // Public
        joblist_t to_provide_jobs;

        // Private
        joblist_t provided_jobs;
    };

    struct JobProviderSystem : ecs::SubSystem<JobProviderSystem, JobProvider> {
        void update_item(ecs::Ent*, JobProvider*);

        static ecs::CRTPSystemFactory<JobProviderSystem> factory;
    };

}