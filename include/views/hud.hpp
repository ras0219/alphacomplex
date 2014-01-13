#pragma once

#include "widget.hpp"
#include "defs.hpp"

#include <string>
#include <deque>

using std::string;
using std::deque;

extern int influence;

struct Hud : StaticWidget<Hud> {
  void render(struct Graphics& g, render_box const& pos);

  static Hud instance;
};

struct A11s {
  A11s() : msgs{ { "", "", "" } } { }

  void announce(const string&);

  deque<string> msgs;

  static A11s instance;
};
