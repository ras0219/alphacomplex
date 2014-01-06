#pragma once

#include "city.hpp"
#include "view.hpp"
#include "mapview.hpp"

struct DesignView : View {
  DesignView(struct ViewStack* vs, MapView& mv, City& c)
  : vstk(vs), mv(mv), city(c) {}

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;
  MapViewCursor mv;
  City& city;
};
