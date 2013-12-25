#pragma once

#include "entity.hpp"
#include "ai.hpp"
#include "subsystem.hpp"
#include "foodstuff.hpp"

struct NeedsAI : AspectStatic<Aspect::NeedsAI, NeedsAI> {
  int food = 100;
  int max_food = 100;

  int sleep = 100;
  int max_sleep = 100;

  int happy = 100;
  int max_happy = 100;
};

struct NeedsSystem : SubSystem<NeedsSystem, NeedsAI> {
  NeedsSystem() : SubSystem(100) { }

  inline void update(value_type& p) {
    AI& ai = *p.first->assert_get<AI>();

    NeedsAI& nai = *get<0>(p.second);
    if (nai.food <= nai.max_food / 5 && ai.priority() < 5) {
      //ai.interrupt(new SeekFoodAI, 5);
      announce("Hunger...");
      return;
    }
  }
};
