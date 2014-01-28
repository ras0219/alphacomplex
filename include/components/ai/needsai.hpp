#pragma once

#include "entities/entity.hpp"
#include "ai.hpp"
#include "entities/subsystem.hpp"
#include "components/foodstuff.hpp"

struct Needs : ComponentCRTP<Component::Needs, Needs> {
  int food = 20;
  int max_food = 20;

  int sleep = 20;
  int max_sleep = 20;

  int happy = 20;
  int max_happy = 20;
};

struct NeedsSystem : SubSystem<NeedsSystem, Needs, AI> {
  NeedsSystem() : SubSystem(100) { }

  void update_item(Ent* e, Needs* nai, AI* ai);
};

extern NeedsSystem needssystem;
