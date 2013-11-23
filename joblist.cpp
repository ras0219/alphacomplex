#include "joblist.hpp"
#include "graphics.hpp"
#include "garbage.hpp"

JobList::iterator JobList::add_job(Job* j) {
  jlist.push_front(j);
  return jlist.begin();
}

void JobList::remove_job(JobList::iterator it) {
  jlist.erase(it);
}

Job* JobList::pop_job() {
  Job* j = jlist.back();
  jlist.pop_back();
  return j;
}

void JobListing::render(Graphics& g) {
  //XFillRectangle(g.display, g.window, g.white_gc, x, y, 200, 200);

  g.drawString(x, y+10, title, Graphics::DEFAULT);
  g.drawString(x, y+20, "---------", Graphics::DEFAULT);

  int yoffset = y+30;
  for (auto j : jlist->jlist) {
    char buf[24];
    int nchars = j->description(buf, 24);
    g.drawString(x, yoffset, string(buf));
    yoffset += 10;
  }
}

JobList jobs, active_jobs;
