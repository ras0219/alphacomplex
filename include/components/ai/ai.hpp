#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"

#include <memory>
#include <vector>
using std::vector;

struct AI : AspectStatic<Aspect::AI, AI> {
  using priority_t = int;
  using script_ptr = std::shared_ptr<struct AIScript>;
  using timer_t = int;

  AI(script_ptr base) : scripts(1, { base, 0 }) { }

  // Methods for the public
  void update();
  bool interrupt(script_ptr ais, priority_t prior);

  // Methods for AIScripts
  inline timer_t get_timer() { return timer; }
  timer_t replace_script(script_ptr ais);
  timer_t push_script(script_ptr ais);
  timer_t pop_script();

  inline priority_t priority() const { return scripts.back().second; }
  inline script_ptr& script() { return scripts.back().first; }

  // Data
  timer_t timer;
  vector< pair<script_ptr, priority_t> > scripts;
};

struct AISystem : SubSystem<AISystem, AI> {
  inline void update_item(Ent*, AI* ai) {
    ai->update();
  }
};
extern AISystem aisystem;


struct AIScript {
  virtual ~AIScript() { }

  virtual AI::timer_t start(AI* ai) = 0;
  //this statement prevents unused argument, but has no effect.
  virtual void suspend(AI* ai) {(void)ai; }
  virtual AI::timer_t resume(AI* ai) { return start(ai); }
  virtual AI::timer_t update(AI* ai) { return complete(ai); }

  inline AI::timer_t complete(AI* ai) {
    return ai->pop_script();
  }
};

// These are just some inline method calls, no worries
inline AI::timer_t AI::pop_script() {
  assert(scripts.size() > 1);
  scripts.pop_back();
  return script()->resume(this);
}

inline AI::timer_t AI::push_script(AI::script_ptr ais) {
  script()->suspend(this);
  scripts.emplace_back(std::move(ais), priority());
  return script()->start(this);
}

inline AI::timer_t AI::replace_script(AI::script_ptr ais) {
  script() = std::move(ais);
  return script()->start(this);
}

inline void AI::update() {
  --timer;

  if (timer <= 0) timer = script()->update(this);
}
