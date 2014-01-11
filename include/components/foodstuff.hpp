#pragma once

#include "entities/entity.hpp"

struct Foodstuff : AspectStatic<Aspect::Foodstuff, Foodstuff> {
  Foodstuff(int amt) : amount(amt) { }

  int amount;
};
