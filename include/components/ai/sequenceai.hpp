#pragma once

#include "ai.hpp"

struct SequenceAI : AIScript {
  SequenceAI() : i(0) {}
  SequenceAI(std::initializer_list<AI::script_ptr> il) : subs(il), i(0) { }

  virtual AI::timer_t start(AI* ai);

  inline void add_task(AI::script_ptr s) {
    subs.push_back(std::move(s));
  }

  virtual const std::string& description() const override {
      return desc;
  }

  using ptr = std::shared_ptr<SequenceAI>;

private:
  std::vector<AI::script_ptr> subs;
  unsigned int i;
  std::string desc;
};
