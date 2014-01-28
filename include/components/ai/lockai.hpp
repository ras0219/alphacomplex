#pragma once

#include "ai.hpp"
#include "components/item.hpp"

struct LockAI : AIScript {
  LockAI(Item* item, const AI::script_ptr& sp) : item(item), subscript(sp), lock(nullptr) { }

  virtual AI::timer_t start(AI* ai) override {
    if (lock = item->try_lock())
      return ai->push_script(subscript);
    else
      // On failure, short circuit.
      return complete(ai);
  }

  virtual AI::timer_t resume(AI* ai) override {
    // Release the item lock.
    lock.reset();
    return complete(ai);
  }

  Item* item;
  AI::script_ptr subscript;
  ItemLock lock;
};

inline std::shared_ptr<LockAI> make_lockai(Item* item, const AI::script_ptr& sp) {
  return std::make_shared<LockAI>(item, sp);
}
