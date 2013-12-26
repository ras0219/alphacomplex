#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"

#include <vector>
using std::vector;

struct AI : AspectStatic<Aspect::AI, AI> {
  AI(struct AIScript* base) : scripts(1, { base, 0 }) { }

  // Methods for the public
  void update();
  inline void interrupt(AIScript* s) { push_script(s); }

  // Methods for private
  inline int get_timer() { return timer; }
  int push_script(AIScript* ais);

  void interrupt(AIScript* ais, int prior);

  inline int priority() const { return scripts.back().second; }
  inline struct AIScript*& script() { return scripts.back().first; }

  // Data
  int timer;
  vector< pair<struct AIScript*, int> > scripts;
};

struct AISystem : SubSystem<AISystem, AI> {
  inline void update_item(Ent* e, AI* ai) {
    ai->update();
  }
};
extern AISystem aisystem;


struct AIScript {
  AIScript() : prev(nullptr) {}
  virtual ~AIScript() { }

  inline void set_prev(AIScript* ais) { prev = ais; }
  inline AIScript* get_prev() { return prev; }

  virtual int start(AI* ai) = 0;
  //this statement prevents unused argument, but has no effect.
  virtual void suspend(AI* ai) {(void)ai; }
  virtual int resume(AI* ai) { return start(ai); }
  virtual int update(AI* ai) { return complete(ai); }

  inline int complete(AI* ai) {
    if (prev != nullptr)
      ai->script() = prev;
    else
      ai->scripts.pop_back();
    return ai->script()->resume(ai);
  }

  AIScript* prev;
};

inline int AI::push_script(AIScript* ais) {
  script()->suspend(this);
  ais->set_prev(script());
  script() = ais;
  return ais->start(this);
}
inline void AI::interrupt(AIScript* ais, int prior) {
  script()->suspend(this);
  ais->set_prev(nullptr);
  scripts.emplace_back(ais, prior);
  timer = ais->start(this);
}

inline void AI::update() {
  --timer;

  if (timer <= 0)
    timer = script()->update(this);
}
