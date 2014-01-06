#pragma once

#include "entities/entity.hpp"
#include "ai.hpp"
#include "entities/subsystem.hpp"
#include "components/foodstuff.hpp"

struct NeedsAI : AspectStatic<Aspect::NeedsAI, NeedsAI> {
  int food = 20;
  int max_food = 20;

  int sleep = 20;
  int max_sleep = 20;

  int happy = 20;
  int max_happy = 20;
};

struct NeedsSystem : SubSystem<NeedsSystem, NeedsAI, AI> {
  NeedsSystem() : SubSystem(100) { }

  void update_item(Ent* e, NeedsAI* nai, AI* ai);
};

extern NeedsSystem needssystem;
