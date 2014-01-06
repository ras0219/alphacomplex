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
inline std::shared_ptr<CallbackAI<F>> make_callbackai(const F& f) { return make_shared<CallbackAI<F>>(f); }
