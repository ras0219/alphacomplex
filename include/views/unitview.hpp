#pragma once

#include "baseview.hpp"

struct UnitView : BaseView {
  UnitView(struct ViewStack* vs);

  virtual void render_body(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeySym ks) override;
};
