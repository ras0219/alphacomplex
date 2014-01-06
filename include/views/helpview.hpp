#pragma once

#include "view.hpp"

struct HelpView : View {
  HelpView(struct ViewStack* vs) : vstk(vs) { }

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
};

struct HelpText : Widget {
  virtual void render(Graphics& g);

  static HelpText instance;
};
