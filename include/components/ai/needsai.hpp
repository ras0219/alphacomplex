#pragma once

#include "entities/entity.hpp"
#include "ai.hpp"
#include "entities/subsystem.hpp"
#include "components/foodstuff.hpp"

struct NeedsAI : AspectStatic<Aspect::NeedsAI, NeedsAI> {
  int food = 100;
  int max_food = 100;

  int sleep = 100;
  int max_sleep = 100;

  int happy = 100;
  int max_happy = 100;
};

struct NeedsSystem : SubSystem<NeedsSystem, NeedsAI, AI> {
  NeedsSystem() : SubSystem(100) { }

  inline void update(Ent* e, NeedsAI* nai, AI* ai) {
    if (nai->food <= nai->max_food / 5 && ai->priority() < 5) {
      //ai.interrupt(new SeekFoodAI, 5);
      announce("Hunger...");
      return;
    }
  }
};
