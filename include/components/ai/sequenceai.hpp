#pragma once

#include "ai.hpp"

struct SequenceAI : AIScript {
  SequenceAI(initializer_list<AIScript*> il) : subs(il), i(0) { }
  ~SequenceAI() {
    while (i != subs.size())
      delete subs[i++];
  }

  inline virtual int start(AI* ai) {
    if (i == subs.size())
      return complete(ai);
    else
      return ai->push_script(subs[i++]);
  }

  vector<AIScript*> subs;
  uint i;
};
