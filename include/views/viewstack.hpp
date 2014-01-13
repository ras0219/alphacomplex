#pragma once

#include "view.hpp"

#include <vector>

using std::vector;

struct ViewStack : View {
  virtual void render(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeySym ks) override;

  void push(View*);
  void pop();
  void reset();

  vector<View*> vstack;
};
