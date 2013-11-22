#include "garbage.hpp"

const char* Garbage::RAWNAME = "garbage";

int GarbageJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Clean Garbage @ %d, %d", g->x, g->y);
}

