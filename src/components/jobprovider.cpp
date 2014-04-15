#include "components/jobprovider.hpp"
#include "components/ai/job.hpp"
#include "joblist.hpp"

namespace job {

    JobProvider::~JobProvider() {
        for (auto j : provided_jobs)
            if (j->available())
                j->complete();
    }

    void JobProviderSystem::update_item(ecs::Ent*, JobProvider* jp) {
        for (auto& j : jp->to_provide_jobs) {
            JobList::getJoblist().add_job(j);
            jp->provided_jobs.emplace_back(std::move(j));
        }

        jp->to_provide_jobs.clear();

        // Partition the list of jobs into completed and non-completed.
        // Non-completed jobs are moved to the front of the list.
        auto it = std::partition(jp->provided_jobs.begin(),
                                 jp->provided_jobs.end(),
                                 [](std::shared_ptr<Job> j) { return !j->completed(); });

        // Remove all the completed jobs.
        jp->provided_jobs.erase(it, jp->provided_jobs.end());
    }

    ecs::CRTPSystemFactory<JobProviderSystem> JobProviderSystem::factory;

}