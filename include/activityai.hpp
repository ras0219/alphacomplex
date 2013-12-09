#pragma once

#include "entity.hpp"

struct ActivityAI : AIScript {
  ActivityAI(int d) : delay(d) { }

  virtual int start(AI* ai) {
    return delay;
  }

  int delay;
};
