#pragma once

#include "city.hpp"
#include "view.hpp"

#include "mapview.hpp"
#include "navhelper.hpp"

struct MainView : View {
  MainView(struct ViewStack* vs, City* c);

  virtual void render(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeySym ks) override;

  struct ViewStack* vstk;

  struct HelpView* hview;
  struct UnitView* uview;
  struct AnnounceView* aview;
  struct DesignView* dview;

  City* city;
  MapView mv;
  NavHelper nav;
};
