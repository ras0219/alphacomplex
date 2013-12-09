#pragma once

#include "ai.hpp"

struct SequenceAI : AIScript {
  SequenceAI(initializer_list<AIScript*> il) : subs(il), it(subs.begin()) { }
  ~SequenceAI() {
    while (it != subs.end())
      delete *it++;
  }

  inline virtual int start(AI* ai) {
    return ai->push_script(*it++);
  }
  inline virtual int resume(AI* ai) {
    if (it == subs.end())
      return complete(ai);
    else
      return ai->push_script(*it++);
  }

  vector<AIState*> subs;
  vector<AIState*>::iterator it;
};
