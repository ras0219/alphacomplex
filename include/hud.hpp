#pragma once

#include "component.hpp"

#include <string>

using std::string;

extern int influence;

struct Hud : Component {
  virtual void render(struct Graphics& g);
};

void announce(const string&);
