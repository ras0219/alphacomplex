#pragma once

#include "entity.hpp"

struct ItemProperties {
  string name;
  float mass;
};

struct Item : AspectStatic<Aspect::Item, Item> {
  Item(const ItemProperties& p) : prop(p), { }

  inline void lock() { assert(!locked); locked = true; }
  inline void unlock() { assert(locked); locked = false; }

  const ItemProperties& prop;
  bool locked;
};
