#pragma once

#include "ai.hpp"

template<class CB>
struct IfScript : AIScript {
  IfScript(const CB& f, AIScript* s)
    : cb(f), script(s) { }

  virtual ~IfScript() { delete script; }

  virtual int start(AI* ai) override {
    if (cb()) {
      AIScript* s = script;
      script = nullptr;
      return ai->push_script(s);
    }
    return complete(ai);
  }
  virtual int resume(AI* ai) override { return complete(ai); }

  CB cb;
  AIScript* script;
};

template<class CB>
inline IfScript<CB>* new_ifscript(const CB& f, AIScript* s) {
  return new IfScript<CB>(f, s);
}

template<class CB>
struct IfElseScript : AIScript {
  IfElseScript(const CB& f, AIScript* ts, AIScript* fs)
    : cb(f), truescript(ts), falsescript(fs) { }
  virtual ~IfElseScript() { delete truescript; delete falsescript; }

  virtual int start(AI* ai) override {
    AIScript* s;
    if (cb()) {
      s = truescript;
      truescript = nullptr;
    } else {
      s = falsescript;
      falsescript = nullptr;
    }
    return ai->push_script(s);
  }
  virtual int resume(AI* ai) override { return complete(ai); }

  CB cb;
  AIScript* truescript;
  AIScript* falsescript;
};
