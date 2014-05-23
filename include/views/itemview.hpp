#pragma once

#include <memory>
#include "view.hpp"
#include "mapview.hpp"
#include "navhelper.hpp"

struct City;
struct ViewStack;

struct ItemView : View {
    ItemView(ViewStack* vs, MapView* mv, City* c);

    virtual void render(Graphics& g, render_box const& pos) override;
    virtual void handle_keypress(KeyboardKey ks) override;

    ViewStack* vstk;
    MapViewCursor mv;
    City* city;
    NavHelper nav;

private:
    std::unique_ptr<struct ItemList> ilist;
};
