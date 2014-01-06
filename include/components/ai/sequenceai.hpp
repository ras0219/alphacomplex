#pragma once

#include "ai.hpp"

struct SequenceAI : AIScript {
  SequenceAI() : i(0) {}
  SequenceAI(initializer_list<AI::script_ptr> il) : subs(il), i(0) { }
  ~SequenceAI() { }

  virtual AI::timer_t start(AI* ai);
  inline void add_task(AI::script_ptr s) {
    subs.push_back(std::move(s));
  }

  using ptr = std::shared_ptr<SequenceAI>;

  vector<AI::script_ptr> subs;
  uint i;
};
