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

struct unlock_item {
  inline void operator()(struct Item* i);
};

/// RAII Wrapper to cleanly manipulate item locks.
struct ItemLock {
  /// Construct with no item
  ItemLock() {}
  /// Construct with no item
  ItemLock(std::nullptr_t) {}
  /// Construct with item `i`. Item `i` should be locked before this is called.
  explicit ItemLock(Item* i) : ptr(i) {}
  /// Transfer lock ownership.
  ItemLock(ItemLock&& i) : ptr(std::move(i.ptr)) {}
  /// Locks cannot be assigned.
  ItemLock& operator=(const ItemLock&) = delete;
  /// Transfer lock ownership and release current lock (if any).
  ItemLock& operator=(ItemLock&& i) {
    std::swap(ptr, i.ptr);
    return *this;
  }

  /// Convenience method
  Item& operator*() { return *ptr; }
  /// Convenience method
  Item* operator->() { return ptr.get(); }

  /// Allow the syntax `if (itemlock) { ... }`
  explicit operator bool() { return ptr.get() != nullptr; }

  /// Retrieve the underlying Item* and release responsibility for unlocking
  Item* release() { return ptr.release(); }
  /// Retrieve the underlying Item*
  Item* get() { return ptr.get(); }
  /// Unlock the underlying Item* and release responsibility.
  void reset() { return ptr.reset(); }

  /// Remove the item from its container and return the parent lock
  inline ItemLock remove_from();
  /// Consume another item lock and insert this item into the other.
  inline void insert_into(ItemLock&& o);
  /// Delete the underlying item object. Return the parent lock.
  inline ItemLock delete_reset();

private:
  std::unique_ptr<struct Item, unlock_item> ptr;
};

inline void unlock_item::operator()(Item* i) {
  assert(i->locked);
  i->locked = false;
  if (i->container)
    (*this)(i->container);
}

inline ItemLock ItemLock::remove_from() {
  auto parent = ptr->container;
  ptr->remove_from();
  return ItemLock(parent);
}

inline void ItemLock::insert_into(ItemLock&& o) {
  assert(o);
  ptr->insert_into(o.release());
}

inline ItemLock ItemLock::delete_reset() {
  ItemLock r = remove_from();
  delete get();
  reset();
  return r;
}
