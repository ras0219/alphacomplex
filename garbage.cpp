#include "garbage.hpp"

const char* CleaningRoom::RAWNAME = "cleaningroom";

const char* Garbage::RAWNAME = "garbage";

int GarbageJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Clean Garbage @ %d, %d", g->x, g->y);
}

const char* SupplyJob::RAWNAME = "supply";

int SupplyJob::description(char* buf, size_t n) const {
  return snprintf(buf, n, "Fetch Cleaning Supplies");
}
