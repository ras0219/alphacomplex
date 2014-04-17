#include "views/designview.hpp"

#include "views/viewstack.hpp"
#include "views/mapview.hpp"
#include "views/hud.hpp"
#include "views/statustext.hpp"
#include "views/defaultlayout.hpp"

#include "hydroponics/hydroponics.hpp"
#include "storage/storageroom.hpp"
#include "entities/workroom.hpp"
#include "entities/entity.hpp"
#include "components/room.hpp"

extern bool paused;

namespace {
  enum Mode {
    DigMode,
    CreateRoom,
    CreateRoom2,
    PlaceFurniture
  };
  enum RoomType {
    Hydroponics,
    WorkRoom,
    FilingRoom,
    StorageRoom
  };
}


DesignView::DesignView(ViewStack* vs, MapView* mapv, City* c)
: vstk(vs), mv(mapv), city(c), mode(DigMode)
{
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
  nav.register_key(KEY_Return, "[Ent] Rect/Toggle", [this]() { activate(); });

  nav.register_key(KEY_d, "[d] Dig Mode", [this]() { mode = DigMode; });
  nav.register_key(KEY_h, "[h] Hydroponics Room", [this]() { mode = CreateRoom; rtype = Hydroponics; });
  nav.register_key(KEY_s, "[s] Storage Room", [this]() {  mode = CreateRoom; rtype = StorageRoom; });
  nav.register_key(KEY_w, "[w] Work Room", [this]() {  mode = CreateRoom; rtype = WorkRoom; });
}

void DesignView::render(Graphics& g, render_box const& pos) {
  render_box pos2 = DefaultLayout::render_layout(this, g, pos);

  mv.render(g, pos2);
}

void DesignView::activate() {
    if (mode == DigMode) {
        city->toggle_dig_wall(mv.csr.x, mv.csr.y);
    } else if (mode == CreateRoom) {
        pt1 = mv.csr;
        mode = CreateRoom2;
    } else if (mode == CreateRoom2) {
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
            mode = CreateRoom;
            return;
        }

        if (rtype == Hydroponics) {
            hydroponics::make_hydroponics_room(r);
        }
        if (rtype == StorageRoom) {
            storage::make_storageroom(r);
        }
        if (rtype == WorkRoom) {
            make_workroom(r);
        }
        mode = CreateRoom;
    }
}

void DesignView::handle_keypress(KeyboardKey ks) {
  if (nav.handle_keypress(ks)) return;
  switch (ks) {
  case KEY_Left: return mv.left();
  case KEY_Right: return mv.right();
  case KEY_Up: return mv.up();
  case KEY_Down: return mv.down();
  default:
    break;
  }
}
