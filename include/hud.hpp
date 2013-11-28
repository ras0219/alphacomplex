#pragma once

#include "component.hpp"
#include "defs.hpp"

#include <string>
#include <deque>

using std::string;
using std::deque;

extern int influence;

struct Hud : Component {
  virtual void render(struct Graphics& g);
};

extern Hud hud;

struct A11s {
  A11s() : msgs{ { "", "", "" } } { }

  void announce(const string&);

  deque<string> msgs;
};

extern A11s a11s;
