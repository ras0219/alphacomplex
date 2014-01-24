#pragma once

#include "baseview.hpp"
#include "mapview.hpp"

struct MainView : BaseView {
  MainView(struct ViewStack* vs, struct City* c);

  virtual void render_body(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeyboardKey ks) override;

  struct HelpView* hview;
  struct UnitView* uview;
  struct AnnounceView* aview;
  struct DesignView* dview;

  struct City* city;
  MapView mv;
};
