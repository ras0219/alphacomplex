#pragma once

#include "component.hpp"

extern int influence;

struct Hud : Component {
  virtual void render(struct Graphics& g);
};

