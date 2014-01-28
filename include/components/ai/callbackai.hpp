#pragma once

#include "ai.hpp"

template<class F>
struct CallbackAI : AIScript {
  CallbackAI(const F& g) : f(g) { }

  virtual int start(AI* ai) {
    f(ai);
    return complete(ai);
  }

  F f;
};

template<class F>
inline std::shared_ptr<CallbackAI<F>> make_callbackai(const F& f) {
  return std::make_shared<CallbackAI<F>>(f);
}

// Loop Callback
template<class F>
struct LoopCallbackAI : AIScript {
  LoopCallbackAI(const F& g) : f(g) { }

  virtual int start(AI* ai) override {
    AI::timer_t t = f(ai);
    if (t == 0)
      return complete(ai);
    else
      return t;
  }
  virtual int update(AI* ai) override {
    return start(ai);
  }

  F f;
};

template<class F>
inline std::shared_ptr<LoopCallbackAI<F>> make_loopcallbackai(const F& f) {
  return std::make_shared<LoopCallbackAI<F>>(f);
}
