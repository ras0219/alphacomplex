#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"

#include <memory>
#include <vector>

struct AI : ComponentCRTP<Component::AI, AI> {
  using priority_t = int;
  using script_ptr = std::shared_ptr<struct AIScript>;
  using timer_t = int;

  AI(script_ptr base) : scripts(1, { base, 0 }) { }

  // Methods for the public
  void update();
  bool interrupt(script_ptr ais, priority_t prior);

  // Methods for AIScripts
  inline timer_t get_timer() { return timer; }
  /// Replaces the top script on the stack.
  /// Note: The top script receives no messages.
  timer_t replace_script(script_ptr ais);
  /// Add another script to the stack above the current.
  /// Calls suspend & resume
  timer_t push_script(script_ptr ais);
  /// Pops the top script off the stack.
  /// Note: The top script receives no messages.
  timer_t pop_script();

  inline priority_t priority() const { return scripts.back().second; }
  inline script_ptr& script() { return scripts.back().first; }

  // Data
  timer_t timer;
  std::vector< std::pair<script_ptr, priority_t> > scripts;
};

struct AISystem : SubSystem<AISystem, AI> {
  inline void update_item(Ent*, AI* ai) {
    ai->update();
  }
};
extern AISystem aisystem;


struct AIScript {
  virtual ~AIScript() { }

  /// Executed when a script starts for the first time
  virtual AI::timer_t start(AI* ai) = 0;
  /// Executed when a script should sleep.
  /// Default behavior: none
  virtual void suspend(AI*) { }
  /// Executed after a suspend
  /// Default behavior: start()
  virtual AI::timer_t resume(AI* ai) { return start(ai); }
  /// Executed after a resume or start.
  /// Default behavior: complete()
  virtual AI::timer_t update(AI* ai) { return complete(ai); }

  /// Called by the script to complete itself.
  inline AI::timer_t complete(AI* ai) {
    return ai->pop_script();
  }
};

// These are just some inline method calls, no worries
inline AI::timer_t AI::pop_script() {
  if (scripts.size() <= 1)
    assert(false);
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
