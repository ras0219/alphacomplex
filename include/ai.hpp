#pragma once

#include "entity.hpp"
#include "subsystem.hpp"

struct AI : AspectStatic<Aspect::AI, AI> {
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

struct AISystem : SubSystem<AISystem, AI> {
  inline void update(value_type& p) {
    get<0>(p.second)->update();
  }
};
extern AISystem aisystem;


struct AIScript {
  AIScript() : prev(nullptr) {}
  virtual ~AIScript() { }

  inline void set_prev(AIScript* ais) { prev = ais; }
  inline AIScript* get_prev() { return prev; }

  virtual int start(AI* ai) = 0;
  virtual void suspend(AI* ai) { }
  virtual int resume(AI* ai) { return start(ai); }
  virtual int update(AI* ai) { return complete(ai); }

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
