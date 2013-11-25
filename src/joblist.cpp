#include "joblist.hpp"
#include "graphics.hpp"
#include "garbage.hpp"

#include <iostream>

JobList::iterator JobList::add_job(Job* j) {
  jlist.push_back(j);
  return --jlist.end();
}

void JobList::remove_job(JobList::iterator it) {
  jlist.erase(it);
}

Job* JobList::pop_job(Security::Mask secmask,
                      Department::Mask deptmask) {
  for (auto it = jlist.begin(); it != jlist.end(); ++it) {
    Job *j = *it;
    if (j->security() & secmask && j->department() & deptmask) {
      jlist.erase(it);
      return j;
    }
  }
  return nullptr;
}

void JobListing::render(Graphics& g) {
  //XFillRectangle(g.display, g.window, g.white_gc, x, y, 200, 200);

  g.drawString(g.getWidth() - 200, y+10, title, Graphics::DEFAULT);
  g.drawString(g.getWidth() - 200, y+20, "---------", Graphics::DEFAULT);

  int yoffset = y+30;
  for (auto j : jlist->jlist) {
    char buf[24];
    j->description(buf, 24);
    g.drawString(g.getWidth() - 200, yoffset, string(buf));
    yoffset += 10;
  }
}

JobList jobs, active_jobs;
