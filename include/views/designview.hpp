#pragma once

#include <memory>
#include "view.hpp"
#include "mapview.hpp"
#include "navhelper.hpp"

struct City;
struct ViewStack;
struct FurnitureDesignView;
struct RoomsDesignView;

struct DesignView : View {
  DesignView(ViewStack* vs, MapView* mv, City* c);

  virtual void render(Graphics& g, render_box const& pos) override;
  virtual void handle_keypress(KeyboardKey ks) override;

  void build_wall();
  void remove_wall();

  ViewStack* vstk;
  MapViewCursor mv;
  City* city;
  NavHelper nav;

private:
    std::unique_ptr<FurnitureDesignView> fdv;
    std::unique_ptr<RoomsDesignView> rdv;
};
