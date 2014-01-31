#include "components/item.hpp"
#include "components/position.hpp"
#include "components/furniture.hpp"

Point Item::pos() const {
  if (container != nullptr) {
    return container->pos();
  } else {
    if (parent->has<struct Position>()) {
      return parent->assert_get<struct Position>()->as_point();
    }
    if (parent->has<struct Furniture>()) {
      return parent->assert_get<struct Furniture>()->as_point();
    }
    assert(false);
    std::terminate();
  }
}

void Item::on_remove() {
  if (container) remove_from();

  // For now, just terminate the program if we try to delete a filled container
  if (!items.empty()) {
    assert(false);
    std::terminate();
  }
}

ItemLock Item::try_lock() {
  if (locked)
    return ItemLock(nullptr);
  if (container == nullptr) {
    assert(!locked);
    locked = true;
    return ItemLock(this);
  }
  auto l = container->try_lock();
  if (l) {
    l.release();
    assert(!locked);
    locked = true;
    return ItemLock(this);
  }
  return ItemLock(nullptr);
}

std::vector<ItemLock> ItemLock::to_delete;
