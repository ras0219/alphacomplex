#pragma once

#include "entities/entity.hpp"

struct Foodstuff : ComponentCRTP<Component::Foodstuff, Foodstuff> {
  Foodstuff(int amt) : amount(amt) { }

  int amount;
};
