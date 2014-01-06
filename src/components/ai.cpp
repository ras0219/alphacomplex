#include "components/ai/ai.hpp"

bool AI::interrupt(std::shared_ptr<AIScript> ais, AI::priority_t prior) {
  if (prior <= priority())
    return false;
  script()->suspend(this);
  scripts.emplace_back(std::move(ais), prior);
  timer = script()->start(this);
  return true;
}

AISystem aisystem;
