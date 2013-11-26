#pragma once

#include "view.hpp"

struct UnitView : View {
  UnitView(struct ViewStack* vs) : vstk(vs) { }

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
};
