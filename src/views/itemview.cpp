#include "views/itemview.hpp"

#include "views/viewstack.hpp"
#include "views/mapview.hpp"
#include "views/hud.hpp"
#include "views/statustext.hpp"
#include "views/defaultlayout.hpp"
#include "views/itemlist.hpp"

#include "views/roomsview.hpp"
#include "views/furnitureview.hpp"

#include "entities/entity.hpp"
#include "components/room.hpp"

extern bool paused;

ItemView::ItemView(ViewStack* vs, MapView* mapv, City* c)
    : vstk(vs), mv(mapv), city(c), ilist(std::make_unique<ItemList>(mv.csr))
{
    nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
    nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
}

void ItemView::render(Graphics& g, render_box const& pos) {
    render_box pos2 = DefaultLayout::render_layout(this, g, pos);
    {
        render_box pos3 = pos2;
        pos3.shrink_right(32);
        mv.render(g, pos3);
    }
    pos2.set_width(32, render_box::right);
    ilist->render(g, pos2);
}

void ItemView::handle_keypress(KeyboardKey ks) {
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
