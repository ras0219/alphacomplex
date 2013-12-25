#pragma once

#include "view.hpp"

#include <vector>

using std::vector;

struct ViewStack : View {
  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  void push(View*);
  void pop();
  void reset();

  vector<View*> vstack;
};
