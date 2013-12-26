#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"

#include <vector>
using std::vector;

struct AI : AspectStatic<Aspect::AI, AI> {
  using priority_t = int;

  AI(struct AIScript* base) : scripts(1, { base, 0 }) { }

  // Methods for the public
  void update();
  inline bool interrupt(AIScript* ais, priority_t prior);

  // Methods for private
  inline int get_timer() { return timer; }
  int push_script(AIScript* ais);

  inline int priority() const { return scripts.back().second; }
  inline struct AIScript*& script() { return scripts.back().first; }

  // Data
  int timer;
  vector< pair<struct AIScript*, priority_t> > scripts;
};

struct AISystem : SubSystem<AISystem, AI> {
  inline void update_item(Ent*, AI* ai) {
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

// These are just some inline method calls, no worries

inline int AI::push_script(AIScript* ais) {
  script()->suspend(this);
  ais->set_prev(script());
  script() = ais;
  return ais->start(this);
}
inline bool AI::interrupt(AIScript* ais, int prior) {
  if (prior <= priority())
    return false;

  script()->suspend(this);
  ais->set_prev(nullptr);
  scripts.emplace_back(ais, prior);
  timer = ais->start(this);
  return true;
}

inline void AI::update() {
  --timer;

  if (timer <= 0)
    timer = script()->update(this);
}
