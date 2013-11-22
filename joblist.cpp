#include "joblist.hpp"

void JobList::add_job(Job* j) {
  j->next = head;
  j->prev = nullptr;
  if (head != nullptr)
    head->prev = j;
}

void JobList::remove_job(Job* j) {
  if (j->next != nullptr)
    j->next->prev = j->prev;
  if (j->prev != nullptr)
    j->prev->next = j->next;
  else
    head = j->next;
}

Job* JobList::pop_job() {
  Job* r = head;
  remove_job(r);
  return r;
}

JobList jobs;
