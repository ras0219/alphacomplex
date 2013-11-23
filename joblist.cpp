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

  XDrawString(g.display, g.window, DefaultGC(g.display, g.s),
              x, y + 10,
              title.c_str(), title.size());
  XDrawString(g.display, g.window, DefaultGC(g.display, g.s),
              x, y + 20,
              "---------", 9);

  int yoffset = y+30;
  for (auto j : jlist->jlist) {
    char buf[24];
    int nchars = j->description(buf, 24);
    XDrawString(g.display, g.window, DefaultGC(g.display, g.s),
                x, yoffset,
                buf, nchars);
    yoffset += 10;
  }
}

Job* JobStep::complete_step() {
  Job* j = next_step;
  next_step = nullptr;
  return j;
}

JobList jobs, active_jobs;
