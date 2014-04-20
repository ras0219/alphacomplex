#include "views/furnitureview.hpp"
#include "views/designview.hpp"
#include "views/defaultlayout.hpp"
#include "views/viewstack.hpp"

#include "hydroponics/hydroponics_table.hpp"

namespace {
    enum FurnitureType {
        HydroponicsTable
    };
}

extern bool paused;

FurnitureDesignView::FurnitureDesignView(DesignView* dv_)
: dv(dv_), ftype(HydroponicsTable)
{
    nav.register_key(KEY_Escape, "[Esc] Back", [this]() { dv->vstk->pop(); });
    nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
    nav.register_key(KEY_Return, "[Ent] Place Furniture", [this]() { activate(); });

    nav.register_key(KEY_h, "[k] Hydroponics Table", [this]() { ftype = HydroponicsTable; });
}

void FurnitureDesignView::render(Graphics& g, render_box const& pos) {
    render_box pos2 = DefaultLayout::render_layout(this, g, pos);

    dv->mv.render(g, pos2);
}

void FurnitureDesignView::activate() {
    Point p = { dv->mv.csr.city, dv->mv.csr.x, dv->mv.csr.y };
    if (p.city->furniture(p.x, p.y) != nullptr) {
        // There is already furniture there
        announce("There is already furniture there.");
        return;
    }

    switch (ftype) {
    case HydroponicsTable:
        hydroponics::make_hydroponics_table(p);
        break;
    default:
        std::abort();
    }
}

void FurnitureDesignView::handle_keypress(KeyboardKey ks) {
    if (nav.handle_keypress(ks)) return;
    auto& mv = dv->mv;
    switch (ks) {
    case KEY_Left: return mv.left();
    case KEY_Right: return mv.right();
    case KEY_Up: return mv.up();
    case KEY_Down: return mv.down();
    default:
        break;
    }
}
