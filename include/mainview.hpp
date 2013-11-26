#pragma once

#include "view.hpp"

struct MainView : View {
  MainView(struct ViewStack* vs);

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;

  struct HelpView* hview;
  struct UnitView* uview;
};
