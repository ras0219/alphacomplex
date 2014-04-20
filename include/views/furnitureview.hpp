#pragma once

#include "view.hpp"
#include "navhelper.hpp"

struct DesignView;

struct FurnitureDesignView : View {
    FurnitureDesignView(DesignView* dv_);

    virtual void render(Graphics& g, render_box const& pos) override;
    virtual void handle_keypress(KeyboardKey ks) override;

    void activate();

    DesignView* dv;
    NavHelper nav;

private:
    unsigned int ftype;
};
