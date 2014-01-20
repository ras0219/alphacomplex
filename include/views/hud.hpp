#pragma once

#include "widget.hpp"
#include "defs.hpp"

#include <string>
#include <deque>

using std::string;
using std::deque;

/// Global count of the player's influence
extern int influence;

/// Singleton widget for rendering announcements, gametime, and other standard HUD features.
struct Hud : StaticWidget<Hud> {
  void render(struct Graphics& g, render_box const& pos);

  /// Singleton instance. Use this instead of creating individual ones, since there is no state.
  static Hud instance;
};

/// Singleton tracking announcement messages.
struct A11s {
  A11s() : msgs{ { "", "", "" } } { }

  /// Add a new announcement.
  void announce(const string&);

  deque<string> msgs;

  /// Singleton instance. Use this instead of creating individual ones, since there is no state.
  static A11s instance;
};
