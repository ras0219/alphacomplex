#include "views/roomsview.hpp"
#include "views/defaultlayout.hpp"
#include "views/designview.hpp"
#include "views/viewstack.hpp"

#include "components/room.hpp"

#include "hydroponics/hydroponics.hpp"
#include "storage/storageroom.hpp"
#include "entities/workroom.hpp"

extern bool paused;

namespace {
    enum RoomType {
        Hydroponics,
        WorkRoom,
        FilingRoom,
        StorageRoom
    };
}

RoomsDesignView::RoomsDesignView(DesignView* dv_)
: dv(dv_), select_stage(false)
{
    nav.register_key(KEY_Escape, "[Esc] Back", [this]() { dv->vstk->pop(); });
    nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
    nav.register_key(KEY_Return, "[Ent] Place Rect", [this]() { activate(); });

    nav.register_key(KEY_h, "[h] Hydroponics Room", [this]() { select_stage = false; rtype = Hydroponics; });
    nav.register_key(KEY_s, "[s] Storage Room", [this]() { select_stage = false; rtype = StorageRoom; });
    nav.register_key(KEY_w, "[w] Work Room", [this]() { select_stage = false; rtype = WorkRoom; });
}

void RoomsDesignView::render(Graphics& g, render_box const& pos) {
    render_box pos2 = DefaultLayout::render_layout(this, g, pos);

    dv->mv.render(g, pos2);
}

void RoomsDesignView::activate() {
    auto& mv = dv->mv;
    if (!select_stage) {
        pt1 = mv.csr;
        select_stage = true;
    } else if (select_stage) {
        select_stage = false;

        int tlx = min(pt1.x, mv.csr.x);
        int tly = min(pt1.y, mv.csr.y);
        int brx = max(pt1.x, mv.csr.x);
        int bry = max(pt1.y, mv.csr.y);
        Rect r = { pt1.city, tlx, tly, brx - tlx + 1, bry - tly + 1 };

        // Ensure that there is no overlap with an existing room.
        for (auto room : r.city->rooms) {
            if (room->r.x > r.x + r.w || room->r.x + room->r.w < r.x)
                continue;
            if (room->r.y > r.y + r.h || room->r.y + room->r.h < r.y)
                continue;
            // There's an overlap.
            announce("Cannot place room -- Overlaps with existing.");
            return;
        }

        switch (rtype) {
        case Hydroponics:
            hydroponics::make_hydroponics_room(r);
            break;
        case StorageRoom:
            storage::make_storageroom(r);
            break;
        case WorkRoom:
            make_workroom(r);
            break;
        default:
            std::abort();
        }
    }
}

void RoomsDesignView::handle_keypress(KeyboardKey ks) {
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
