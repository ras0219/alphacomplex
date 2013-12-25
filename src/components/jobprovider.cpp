#include "components/jobprovider.hpp"
#include "components/ai/job.hpp"
#include "joblist.hpp"

JobProvider::~JobProvider() {
  for (auto j : to_provide_jobs)
    delete j;

  for (auto j : provided_jobs)
    if (j->available())
      j->complete();
}

void JobProviderSystem::update(value_type& p) {
  JobProvider& jp = *p.second;

  for (auto j : jp.to_provide_jobs) {
    jp.provided_jobs.emplace_back(j);
    jobs.add_job(jp.provided_jobs.back());
  }

  jp.to_provide_jobs.clear();
}

JobProviderSystem jpsystem;