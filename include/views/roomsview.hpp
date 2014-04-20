#pragma once

#include "view.hpp"
#include "navhelper.hpp"
#include "cursor.hpp"

struct DesignView;

struct RoomsDesignView : View {
    RoomsDesignView(DesignView* dv_);

    virtual void render(Graphics& g, render_box const& pos) override;
    virtual void handle_keypress(KeyboardKey ks) override;

    void activate();

    DesignView* dv;
    NavHelper nav;

private:
    bool select_stage;
    unsigned int rtype;
    CityCursor pt1;
};
