#pragma once

#include "view.hpp"

struct AnnounceView : View {
  AnnounceView(struct ViewStack* vs) : vstk(vs) { }

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
};
