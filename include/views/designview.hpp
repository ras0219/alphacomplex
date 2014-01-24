#pragma once

#include "city.hpp"
#include "view.hpp"
#include "mapview.hpp"
#include "navhelper.hpp"

struct DesignView : View {
  DesignView(struct ViewStack* vs, MapView* mv, City* c);

  virtual void render(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeyboardKey ks) override;

  struct ViewStack* vstk;
  MapViewCursor mv;
  City* city;
  NavHelper nav;
};
