#pragma once

#include "ai.hpp"

struct ActivityAI : AIScript {
  ActivityAI(int d) : delay(d) { }

  virtual int start(AI*) {
    return delay;
  }

  int delay;
};

inline std::shared_ptr<ActivityAI> make_activityai(int d) {
  return std::make_shared<ActivityAI>(d);
}