#pragma once

#include "view.hpp"
#include "navhelper.hpp"

struct AnnounceView : View {
  AnnounceView(struct ViewStack* vs);

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
  NavHelper nav;
};
