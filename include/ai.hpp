#pragma once

#include "entity.hpp"

struct AI : ECompStaticList<EComp::AI> {
  AI(struct AIScript* base) : script(base) { }

  // Methods for the public
  void update();
  inline void interrupt(AIScript* s) { push_script(s); }

  // Methods for private
  inline int get_timer() { return timer; }
  int push_script(AIScript* ais);

  // Data
  int timer;
  struct AIScript* script;
};

struct AIScript {
  AIScript() : prev(nullptr) {}

  inline void set_prev(AIScript* ais) { prev = ais; }
  inline AIScript* get_prev() { return prev; }

  virtual int start(AI* ai) = 0;
  inline virtual void suspend(AI* ai) { }
  inline virtual int resume(AI* ai) { return start(ai); }
  inline virtual int update(AI* ai) { return complete(ai); }

  inline int complete(AI* ai) {
    ai->script = prev;
    return prev->resume(ai);
  }

  AIScript* prev;
};

inline int AI::push_script(AIScript* ais) {
  script->suspend(this);
  ais->set_prev(script);
  script = ais;
  return ais->start(this);
}

inline void AI::update() {
  --timer;

  if (timer <= 0)
    timer = script->update(this);
}
