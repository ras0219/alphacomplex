#pragma once

#include "entities/entity.hpp"
#include "ai.hpp"
#include "entities/subsystem.hpp"
#include "components/foodstuff.hpp"

#include <string>

struct Needs : ComponentCRTP<Component::Needs, Needs> {
  int food = 100;
  int max_food = 200;

  int sleep = 200;
  int max_sleep = 200;

  int happy = 200;
  int max_happy = 200;
};

struct NeedsSystem : SubSystem<NeedsSystem, Needs, AI> {
  NeedsSystem() : SubSystem(100) { }

  void update_item(Ent* e, Needs* nai, AI* ai);
};
