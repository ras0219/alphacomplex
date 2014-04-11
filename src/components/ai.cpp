#include "components/ai/ai.hpp"

AISystem aisystem;

bool AI::interrupt(std::shared_ptr<AIScript> ais, AI::priority_t prior) {
  if (prior <= priority())
    return false;
  script()->suspend(this);
  scripts.emplace_back(std::move(ais), prior);
  timer = script()->start(this);
  return true;
}


// These are just some method calls, no worries
AI::timer_t AI::pop_script() {
  if (scripts.size() <= 1)
    assert(false);
  scripts.pop_back();
  return script()->resume(this);
}

AI::timer_t AI::push_script(AI::script_ptr ais) {
  script()->suspend(this);
  scripts.emplace_back(std::move(ais), priority());
  return script()->start(this);
}

AI::timer_t AI::replace_script(AI::script_ptr ais) {
  script() = std::move(ais);
  return script()->start(this);
}

void AI::update() {
  --timer;

  if (timer <= 0) timer = script()->update(this);
}

AIScript::~AIScript() { }

/// Executed when a script should sleep.
/// Default behavior: none
void AIScript::suspend(AI*) { }
/// Executed after a suspend
/// Default behavior: start()
AI::timer_t AIScript::resume(AI* ai) { return start(ai); }
/// Executed after a resume or start.
/// Default behavior: complete()
AI::timer_t AIScript::update(AI* ai) { return complete(ai); }

/// Called by the script to complete itself.
AI::timer_t AIScript::complete(AI* ai) {
  return ai->pop_script();
}

void AISystem::update_item(Ent*, AI* ai) {
  ai->update();
}
