#pragma once

#include "ai.hpp"

template<class CB>
struct IfScript : AIScript {
  IfScript(const CB& f, AI::script_ptr s)
    : cb(f), script(s) { }

  ~IfScript() {}

  virtual int start(AI* ai) override {
    if (cb(ai)) {
      return ai->replace_script(script);
    }
    return complete(ai);
  }

  CB cb;
  AI::script_ptr script;
};

template<class CB>
inline std::shared_ptr<IfScript<CB>> new_ifscript(const CB& f, AI::script_ptr s) {
  return make_shared<IfScript<CB>>(f, s);
}

template<class CB>
struct IfElseScript : AIScript {
  IfElseScript(const CB& f, AI::script_ptr ts, AI::script_ptr fs)
    : cb(f), truescript(ts), falsescript(fs) { }
  virtual ~IfElseScript() { delete truescript; delete falsescript; }

  virtual int start(AI* ai) override {
    if (cb(ai)) {
      return ai->push_script(truescript);
    } else {
      return ai->push_script(falsescript);
    }
  }
  virtual int resume(AI* ai) override { return complete(ai); }

  CB cb;
  AI::script_ptr truescript;
  AI::script_ptr falsescript;
};
