#include "defs.hpp"
#include "views/announceview.hpp"
#include "views/viewstack.hpp"
#include "views/hud.hpp"
#include "graphics.hpp"
#include "views/statustext.hpp"
#include "views/defaultlayout.hpp"

#include <algorithm>

AnnounceView::AnnounceView(ViewStack* vs) : vstk(vs) {
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
}

void AnnounceView::render(Graphics& g, render_box const& pos) {
  render_box pos2 = DefaultLayout::render_layout(this, g, pos);

  auto it = A11s::instance.msgs.rbegin();
  auto e = it;
  const size_t num_lines = pos2.h;
  if (A11s::instance.msgs.size() < num_lines)
    e = A11s::instance.msgs.rend();
  else
    advance(e, num_lines);

  int r = num_lines;
  while (it != e) {
    g.drawString(pos2.x, pos2.y + r - 1, *it, Graphics::DEFAULT);
    --r;
    ++it;
  }
}

void AnnounceView::handle_keypress(KeySym ks) {
  if (nav.handle_keypress(ks)) return;
}
