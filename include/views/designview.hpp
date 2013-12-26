#pragma once

#include "city.hpp"
#include "view.hpp"

struct DesignView : View {
  DesignView(struct ViewStack* vs, struct MapView* m, City& c);

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
  struct MapView* mv;
  City& city;
};
