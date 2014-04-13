#pragma once

#include "ai.hpp"

template<class CB>
struct IfScript : AIScript {
  IfScript(const CB& f, AI::script_ptr s, std::string d = "")
    : cb(f), script(s), desc(d) { }

  virtual AI::timer_t start(AI* ai) override {
    if (cb(ai)) {
      return ai->push_script(script);
    }
    return complete(ai);
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
  CB cb;
  AI::script_ptr script;
  std::string desc;
};

template<class CB>
inline std::shared_ptr<IfScript<CB>> new_ifscript(const CB& f, AI::script_ptr s) {
  return make_shared<IfScript<CB>>(f, s);
}

template<class CB>
struct IfElseScript : AIScript {
    IfElseScript(const CB& f, AI::script_ptr ts, AI::script_ptr fs, std::string d = "")
        : cb(f), truescript(ts), falsescript(fs), desc(d) { }

  virtual AI::timer_t start(AI* ai) override {
    if (cb(ai)) {
      return ai->push_script(truescript);
    } else {
      return ai->push_script(falsescript);
    }
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
  CB cb;
  AI::script_ptr truescript;
  AI::script_ptr falsescript;
  std::string desc;
};
