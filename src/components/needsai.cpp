#include "components/ai/needsai.hpp"
#include "components/ai/activityai.hpp"

std::shared_ptr<AIScript> make_seek_food_script(Ent* e) {
  return make_shared<ActivityAI>(100);
}

void NeedsSystem::update_item(Ent* e, NeedsAI* nai, AI* ai) {
  if (nai->food <= nai->max_food / 5 && ai->priority() < 5) {
    ai->interrupt(make_seek_food_script(e), 5);
    announce("Hunger...");
    return;
  }

  if (nai->food > 0)
    --nai->food;
}

NeedsSystem needssystem;
