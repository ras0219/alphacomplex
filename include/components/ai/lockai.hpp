#pragma once

#include "ai.hpp"
#include "components/item.hpp"
#include "components/itemlock.hpp"

struct LockAI : AIScript {
  LockAI(Item* item, const AI::script_ptr& sp, std::string d = "") : item(item), subscript(sp), desc(d) { }

  virtual AI::timer_t start(AI* ai) override {
    if ((lock = item->try_lock()))
      return ai->push_script(subscript);
    else
      // On failure, short circuit.
      return complete(ai);
  }

  virtual AI::timer_t update(AI* ai) override {
    // Release the item lock.
    lock.reset();
    return complete(ai);
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
  Item* item;
  AI::script_ptr subscript;
  ItemLock lock;
  std::string desc;
};

inline std::shared_ptr<LockAI> make_lockai(Item* item, const AI::script_ptr& sp) {
  return std::make_shared<LockAI>(item, sp);
}
