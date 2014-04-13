#pragma once

#include "entities/entity.hpp"
#include "entities/subsystem.hpp"

#include <memory>
#include <vector>

struct AI : ComponentCRTP<Component::AI, AI> {
  using priority_t = int;
  using script_ptr = std::shared_ptr<struct AIScript>;
  using timer_t = int;

  using task_stack = std::vector<script_ptr>;
  using task_queue = std::vector< std::pair<priority_t, task_stack> >;

  AI(script_ptr base) : tasks(1, { 0, { base } }) { }

  /// Entry point for the AI System. Called once per AI tick.
  void update();

  /// Attempt to add a task on top of the existing one.
  /// This will only succeed if the priority is higher than the current priority.
  ///@return True if the interrupt succeeded, False if it failed.
  bool add_task(script_ptr ais, priority_t prior);

  ///@name Methods for the current Task
  ///@{

  /// Add another script to the stack above the current.
  ///@return Delay time requested by the new script
  timer_t push_script(script_ptr ais);
  /// Pops the top script off the stack.
  ///@return Delay time requested by the next script
  timer_t pop_script();

  ///@}

  ///@name Low-level accessor methods
  ///@{
  inline timer_t get_timer() { return timer; }
  inline priority_t current_priority() const { return tasks.back().first; }
  inline script_ptr& current_script() { return tasks.back().second.back(); }
  inline task_stack& current_task() { return tasks.back().second; }
  ///@}

private:
    /// Remaining number of ticks before triggering update()
    timer_t timer;
    /// List of current tasks
    ///@invariant Strictly increasing priority
    task_queue tasks;
};

struct AISystem : SubSystem<AISystem, AI> {
  inline void update_item(Ent*, AI* ai) {
    ai->update();
  }
};

/// Interface to be implemented by AI script coroutines
struct AIScript {
  virtual ~AIScript() { }

  /// Yields a description of the current script for UI use
  ///@return Brief description
  virtual const std::string& description() const = 0;

  /// Executed when a script starts for the first time.
  ///@return Amount of time to wait before calling update()
  virtual AI::timer_t start(AI* ai) = 0;

  /// Executed when the current task is interrupted.
  /// Default behavior: Nothing
  virtual void suspend(AI*) { }

  /// Executed when the current task is resumed.
  /// Default behavior: start()
  ///@return Amount of time to wait before calling update()
  virtual AI::timer_t resume(AI* ai) { return start(ai); }

  /// Executed after a resume, a start, or a child task completed.
  ///@pre start() or resume() was called before this
  ///@return Amount of time to wait before calling update() again
  virtual AI::timer_t update(AI* ai) { return complete(ai); }

  /// Called by the script to complete itself.
  ///@deprecated
  inline AI::timer_t complete(AI* ai) {
      assert(ai->current_script().get() == this);
      return ai->pop_script();
  }
};

// These are just some inline method calls, no worries
inline AI::timer_t AI::pop_script() {
    assert(tasks.size() > 0);
    assert(current_task().size() > 0);
    current_task().pop_back();
    if (current_task().size() == 0) {
        // Resume the previous task
        assert(tasks.size() > 1);
        tasks.pop_back();
        return current_script()->resume(this);
    }
    return current_script()->update(this);
}

inline AI::timer_t AI::push_script(AI::script_ptr ais) {
  current_task().emplace_back(std::move(ais));
  return current_script()->start(this);
}

inline void AI::update() {
  --timer;

  if (timer <= 0)
      timer = current_script()->update(this);
}
