#include "defs.hpp"
#include "views/announceview.hpp"
#include "views/viewstack.hpp"
#include "views/hud.hpp"
#include "graphics.hpp"
#include "views/statustext.hpp"

#include <algorithm>

AnnounceView::AnnounceView(ViewStack* vs) : vstk(vs) {
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
}

void AnnounceView::render(Graphics& g) {
  auto it = a11s.msgs.rbegin();
  auto e = it;
  const size_t num_lines = (g.getHeight() - 10) / 12;
  if (a11s.msgs.size() < num_lines)
    e = a11s.msgs.rend();
  else
    advance(e, num_lines);

  int r = num_lines;
  while (it != e) {
    g.drawString(5, 5+12+12*r, *it, Graphics::DEFAULT);
    --r;
    ++it;
  }

  statustext.render(g);
  nav.render(g);
}

void AnnounceView::handle_keypress(KeySym ks) {
  if (nav.handle_keypress(ks)) return;
}

