#pragma once

#include "components/component.hpp"
#include "utilities/global_set.hpp"
#include "point.hpp"
#include <unordered_set>
#include <cassert>

/// Properties of an item -- name of item, mass of item, material of item, etc.
struct ItemProperties {
  std::string name;
  float mass;
};

/// Component to describe an item which has properties, can be locked, and can be nested with other items.
///
/// Invariant: If container is not null, then parent does not have a Position Component or a Furniture Component.
/// Invariant: If container is null, then parent has a Position Component or a Furniture Component.
struct Item : ComponentCRTP<Component::Item, Item>, private global_set<Item> {
  Item(const ItemProperties& p) : prop(p), locked(false), container(nullptr) { }

  virtual void on_remove() override;

  /// Attempt to lock the item. If the lock fails, return a default-initialized `ItemLock`.
  struct ItemLock try_lock();

  inline void insert(Item* i) { items.insert(i); }
  inline void erase(Item* i) { items.erase(i); }

  /// Unsafe function. Should not be called if you do not have both `this` and `container` locked.
  inline void remove_from() {
    assert(container);
    container->erase(this);
    container = nullptr;
  }
  /// Unsafe function. Should not be called if you do not have both `this` and `i` locked.
  inline void insert_into(Item* i) {
    assert(container == nullptr);
    container = i;
    container->insert(this);
  }

  /// Walk up the containment tree and find the top parent's location. Slow O(D) where D is tree depth.
  Point pos() const;

  ItemProperties const& prop;
  bool locked;
  Item* container;
  using set_t = std::unordered_set<struct Item*>;
  set_t items;

  friend struct global_set<Item>;
};
