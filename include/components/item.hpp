#pragma once

#include "entities/entity.hpp"
#include "utilities/global_set.hpp"

struct ItemProperties {
  string name;
  float mass;
};

struct Item : AspectStatic<Aspect::Item, Item>, private global_set<Item> {
  Item(const ItemProperties& p) : prop(p), locked(false) { }

  inline void lock() { assert(!locked); locked = true; }
  inline void unlock() { assert(locked); locked = false; }

  const ItemProperties& prop;
  bool locked;

  template<class> friend struct global_set;
};
