#include "joblist.hpp"
#include "graphics.hpp"
#include "garbage.hpp"

void JobList::add_job(Job* j) {
  j->next = head;
  j->prev = nullptr;
  if (head != nullptr)
    head->prev = j;
  head = j;
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

void JobListing::render(Graphics& g) {
  XFillRectangle(g.display, g.window, g.white_gc, x, y, 200, 200);

  XDrawString(g.display, g.window, DefaultGC(g.display, g.s),
              x, y + 10,
              title.c_str(), title.size());
  XDrawString(g.display, g.window, DefaultGC(g.display, g.s),
              x, y + 20,
              "---------", 9);

  int yoffset = y+30;
  Job* j = jlist->head;
  while (j != nullptr) {
    char buf[24];
    int nchars = snprintf(buf, 24, "Clean Garbage @ %d, %d",
                          j->as<GarbageJob>().x,
                          j->as<GarbageJob>().y);
    XDrawString(g.display, g.window, DefaultGC(g.display, g.s),
                x, yoffset,
                buf, nchars);
    yoffset += 10;
    j = j->next;
  }
}

JobList jobs, active_jobs;
