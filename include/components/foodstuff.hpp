#pragma once

#include "entities/entity.hpp"

struct Foodstuff : ecs::ComponentCRTP<ecs::Component::Foodstuff, Foodstuff> {
  Foodstuff(int amt) : amount(amt) { }

  int amount;
};
