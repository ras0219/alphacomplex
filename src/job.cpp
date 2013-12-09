#include "job.hpp"
#include "citizen.hpp"

int AIState::resume(Citizen* c, AIState*) {
  return start(c);
}

void AIState::suspend(Citizen* c, AIState*) { }

int AIState::complete(Citizen* c) {
  c->aistate = parent;
  delete this;
  return c->aistate->resume(c, this);
}


int SequenceAI::start(Citizen* c) {
  return c->push_aistate(*it);
}
int SequenceAI::update(Citizen*) {
  assert(false);
  return -1;
}
int SequenceAI::resume(Citizen* c, AIState* ai) {
  assert(ai == *it);
  ++it;
  if (it == subs.end())
    return complete(c);
  else
    return start(c);
}
