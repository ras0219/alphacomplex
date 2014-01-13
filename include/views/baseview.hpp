#pragma once

#include "view.hpp"
#include "navhelper.hpp"

/// A common base implementation for views. Most views should use this layout
struct BaseView : View {
  BaseView(struct ViewStack* vs);

  virtual void render(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeySym ks) override;

  struct ViewStack* vstk;
  NavHelper nav;

  /// Renders the body of the child layout
  virtual void render_body(Graphics& g, render_box const& pos) = 0;
};
