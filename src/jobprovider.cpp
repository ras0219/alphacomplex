#include "jobprovider.hpp"
#include "job.hpp"
#include "joblist.hpp"

JobProvider::~JobProvider() {
  for (auto j : to_provide_jobs)
    delete j;

  for (auto j : provided_jobs)
    j->complete();
}

void JobProviderSystem::update(value_type& p) {
  JobProvider& jp = *get<0>(p.second);

  for (auto j : to_provide_jobs) {
    jobs.add_job(j);
    provided_jobs.push_back(j);
  }

  to_provide_jobs.clear();
}
