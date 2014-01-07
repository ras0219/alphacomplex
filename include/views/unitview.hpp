#pragma once

#include "view.hpp"
#include "navhelper.hpp"

struct UnitView : View {
  UnitView(struct ViewStack* vs);

  virtual void render(Graphics& g);

  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
  NavHelper nav;
};
