#pragma once

#include "components/component.hpp"
#include "utilities/global_set.hpp"
#include "point.hpp"
#include <unordered_set>
#include <cassert>
#include <memory>

/// Properties of an item -- name of item, mass of item, material of item, etc.
struct ItemProperties {
  std::string name;
  float mass;
};

struct unlock_item {
  inline void operator()(struct Item* i);
};

using ItemLock = std::unique_ptr<struct Item, unlock_item>;

/// Component to describe an item which has properties, can be locked, and can be nested with other items.
///
/// Invariant: If container is not null, then parent does not have a Position Component or a Furniture Component.
/// Invariant: If container is null, then parent has a Position Component or a Furniture Component.
struct Item : ComponentCRTP<Component::Item, Item>, private global_set<Item> {
  Item(const ItemProperties& p) : prop(p), locked(false), container(nullptr) { }

  inline virtual void on_remove() override {
    if (container) remove_from();

    // For now, just terminate the program if we try to delete a filled container
    if (!items.empty()) {
      assert(false);
      std::terminate();
    }
  }

  inline ItemLock try_lock() {
    if (locked)
      return nullptr;
    if (container == nullptr)
      return ItemLock(this);
    auto l = container->try_lock();
    if (l) {
      l.release();
      assert(!locked);
      locked = true;
      return ItemLock(this);
    }
    return nullptr;
  }

  inline void insert(Item* i) { items.insert(i); }
  inline void erase(Item* i) { items.erase(i); }

  inline void remove_from() {
    assert(container);
    container->erase(this);
    container = nullptr;
  }
  inline void place_into(Item* i) {
    assert(container == nullptr);
    container = i;
    container->insert(this);
  }

  Point pos() const;

  const ItemProperties& prop;
  bool locked;
  Item* container;
  using set_t = std::unordered_set<struct Item*>;
  set_t items;

  friend struct global_set<Item>;
};


inline void unlock_item::operator()(Item* i) {
  assert(i->locked);
  i->locked = false;
  if (i->container)
    (*this)(i->container);
}