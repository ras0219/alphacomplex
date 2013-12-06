#pragma once

#include "city.hpp"
#include "view.hpp"
#include "statustext.hpp"

struct MainView : View {
  MainView(struct ViewStack* vs, City& c);

  virtual void render(Graphics& g);
  virtual void handle_keypress(KeySym ks);

  struct ViewStack* vstk;

  struct HelpView* hview;
  struct UnitView* uview;
  struct AnnounceView* aview;
  struct DesignView* dview;

  StatusText* stext;

  City& city;
};
