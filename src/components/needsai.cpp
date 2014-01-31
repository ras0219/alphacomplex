#include "components/ai/needsai.hpp"
#include "components/ai/activityai.hpp"
#include "components/ai/callbackai.hpp"
#include "utilities/global_set.hpp"
#include "components/item.hpp"
#include "components/foodstuff.hpp"
#include "components/position.hpp"
#include "components/ai/pathai.hpp"

struct SeekFoodAI : AIScript {
  virtual int start(AI* ai) override {
    if (!food) {
      return findfood(ai);
    }

    return findpath(ai);
  }
  virtual int update(AI* ai) override {
    return start(ai);
  }

  int findfood(AI* ai) {
    auto it = global_set<Item>::begin();
    while (it != global_set<Item>::end()) {
      if ((*it)->locked) {
        ++it;
        continue;
      }

      Ent* e = (*it)->parent;
      if (e->has<Foodstuff>()) {
        // I have found food!
        if ((food = (*it)->try_lock())) {
          // It's mine!
          return findpath(ai);
        }
        // I was unable to lock the food.
      }
      ++it;
    }
    // Didn't find food.... :(
    return 200;
  }
  int findpath(AI* ai) {
    assert(food);

    auto dest = food->pos();
    if (dest != ai->parent->get<Position>()->as_point()) {
      // We are not at the food. We must travel!
      return ai->push_script(make_pathai(point{ dest.x, dest.y }));
    }
    // We are at the food! Rejoice!
    return eatfood(ai);
  }

  int eatfood(AI* ai) {
    auto fs = food->parent->get<Foodstuff>();
    auto e = ai->parent;
    auto needs = e->get<Needs>();
    // Consume the food.
    needs->food += fs->amount;
    if (needs->food > needs->max_food)
      needs->food = needs->max_food;
    // Release and delete the food. It has been consumed.
    food.delete_reset();
    return complete(ai);
  }

  ItemLock food;
};

AI::script_ptr make_seek_food_script() {
  return make_shared<SeekFoodAI>();
}

void NeedsSystem::update_item(Ent*, Needs* nai, AI* ai) {
  if (nai->food <= nai->max_food / 5 && ai->priority() < 5) {
    ai->interrupt(make_seek_food_script(), 5);
    return;
  }

  if (nai->food > 0)
    --nai->food;
}

NeedsSystem needssystem;
