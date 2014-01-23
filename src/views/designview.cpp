#include "views/designview.hpp"

#include "views/viewstack.hpp"
#include "views/mapview.hpp"
#include "views/hud.hpp"
#include "views/statustext.hpp"
#include "views/defaultlayout.hpp"

#include "entities/citizen.hpp"

extern bool paused;

DesignView::DesignView(ViewStack* vs, MapView* mapv, City* c)
: vstk(vs), mv(mapv), city(c)
{
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
  nav.register_key(KEY_Return, "[Ent] Dig", [this]() { city->toggle_dig_wall(mv.csr.x, mv.csr.y); });
}

void DesignView::render(Graphics& g, render_box const& pos) {
  render_box pos2 = DefaultLayout::render_layout(this, g, pos);

  mv.render(g, pos2);
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
