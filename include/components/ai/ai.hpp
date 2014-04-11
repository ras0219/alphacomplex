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
  void update_item(Ent*, AI* ai);
};

extern AISystem aisystem;

struct AIScript {
  virtual ~AIScript();
  virtual AI::timer_t start(AI* ai) = 0;
  virtual void suspend(AI*);
  virtual AI::timer_t resume(AI*);
  virtual AI::timer_t update(AI*);
  virtual AI::timer_t complete(AI*);
};

