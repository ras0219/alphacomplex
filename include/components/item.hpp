#pragma once

#include "components/component.hpp"
#include "utilities/global_set.hpp"

#include <cassert>

struct ItemProperties {
  std::string name;
  float mass;
};

/// Component to describe an item which has properties -- name of item, mass of item, material of item, etc.
/// Also provides locking functionality.
struct Item : ComponentCRTP<Component::Item, Item>, private global_set<Item> {
  Item(const ItemProperties& p) : prop(p), locked(false) { }

  inline void lock() { assert(!locked); locked = true; }
  inline void unlock() { assert(locked); locked = false; }

  const ItemProperties& prop;
  bool locked;

  friend struct global_set<Item>;
};
