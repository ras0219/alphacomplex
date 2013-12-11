#include "joblist.hpp"
#include "graphics.hpp"
#include "garbage.hpp"
#include "job.hpp"

#include <iostream>

JobList::iterator JobList::add_job(Job* j) {
  jlist.push_back(j);
  return --jlist.end();
}

void JobList::remove_jobs() {
  auto it = jlist.begin();
  while (it != jlist.end()) {
    if ((*it)->state == Job::COMPLETED) {
      jlist.erase(it++);
      continue;
    }
    ++it;
  }
}

Job* JobList::find_job(Clearance c) {
  auto it = jlist.begin();
  while (it != jlist.end()) {
    Job *j = *it;
    if (j->completed()) {
      jlist.erase(it++);
      continue;
    }
    if (j->available() && j->clearance() & c)
      return j;
    ++it;
  }
  return nullptr;
}

void JobListing::render(Graphics& g) {
  g.drawString(g.getWidth() - 200, y+10, title, Graphics::DEFAULT);
  g.drawString(g.getWidth() - 200, y+20, "---------", Graphics::DEFAULT);

  int yoffset = y+30;
  for (auto j : jlist->jlist) {
    if (j->completed())
      continue;

    char buf[25];
    // Mark in-progress jobs
    *buf = (j->available() ? ' ' : '~');

    j->description(buf+1, 24);
    g.drawString(g.getWidth() - 200, yoffset, string(buf));
    yoffset += 10;
  }
}

JobList jobs;
