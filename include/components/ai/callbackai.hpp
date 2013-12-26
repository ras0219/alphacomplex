#pragma once

#include "ai.hpp"

template<class F>
struct CallbackAI : AIScript {
  CallbackAI(const F& g) : f(g) { }

  virtual int start(AI* ai) {
    f();
    return complete(ai);
  }

  F f;
};

template<class F>
inline CallbackAI<F>* new_callbackai(const F& f) { return new CallbackAI<F>(f); }
