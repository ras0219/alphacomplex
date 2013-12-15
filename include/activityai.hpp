#pragma once

#include "entity.hpp"
#include "ai.hpp"

struct ActivityAI : AIScript {
  ActivityAI(int d) : delay(d) { }

  virtual int start(AI* ai) {
    return delay;
  }

  int delay;
};
