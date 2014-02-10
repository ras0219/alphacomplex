#pragma once

#include "city.hpp"
#include "view.hpp"
#include "mapview.hpp"
#include "navhelper.hpp"
#include "components/itemlock.hpp"

namespace DesignViewE {
  enum Mode;
  enum RoomType;
}

struct DesignView : View {
  DesignView(struct ViewStack* vs, MapView* mv, City* c);

  virtual void render(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeyboardKey ks) override;

  void activate();

  struct ViewStack* vstk;
  MapViewCursor mv;
  City* city;
  NavHelper nav;

private:
  // Mode details
  DesignViewE::Mode mode;
  DesignViewE::RoomType rtype;
  ItemLock ilock;
};
