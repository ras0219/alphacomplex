#pragma once

#include "entity.hpp"

#include <set>

using std::set;

struct Inventory : AspectStatic<Aspect::Inventory, Inventory> {
  Inventory(int c) : capacity(c) { inv.reserve(c); }

  int capacity;
  vector<Ent*> inv;
};
