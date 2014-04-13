#pragma once

#include "ai.hpp"

struct ActivityAI : AIScript {
  ActivityAI(AI::timer_t i, std::string d = "Unknown Activity") : delay(i), desc(d) { }

  virtual AI::timer_t start(AI*) {
    return delay;
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
  AI::timer_t delay;
  std::string desc;
};

inline std::shared_ptr<ActivityAI> make_activityai(int d) {
  return std::make_shared<ActivityAI>(d);
}