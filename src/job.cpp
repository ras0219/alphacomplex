#include "job.hpp"
#include "citizen.hpp"

#include <cassert>

const char* MultiJob::RAWNAME = "multijob";

int MultiJob::description(char* m, size_t n) const {
  assert(!subjobs.empty());
  return subjobs.front()->description(m, n);
}

void MultiJob::assign_task(Citizen* e) {
  assert(!subjobs.empty());
  subjobs.front()->assign_task(e);
}

bool MultiJob::complete_walk(Citizen* e) {
  if (!subjobs.front()->complete_walk(e))
    return false;

  delete subjobs.front();
  subjobs.pop_front();
  if (subjobs.empty())
    return true;

  subjobs.front()->assign_task(e);
  return false;
}

bool MultiJob::complete_activity(Citizen* e) {
  if (!subjobs.front()->complete_activity(e))
    return false;

  delete subjobs.front();
  subjobs.pop_front();
  if (subjobs.empty())
    return true;

  subjobs.front()->assign_task(e);
  return false;
}
