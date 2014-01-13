#pragma once

#include "view.hpp"
#include "navhelper.hpp"

struct UnitView : View {
  UnitView(struct ViewStack* vs);

  virtual void render(Graphics& g, render_box const& pos) override;

  virtual void handle_keypress(KeySym ks) override;

  struct ViewStack* vstk;
  NavHelper nav;
};
