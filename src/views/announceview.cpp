#include "defs.hpp"
#include "views/announceview.hpp"
#include "views/viewstack.hpp"
#include "views/hud.hpp"
#include "graphics.hpp"
#include "views/statustext.hpp"
#include "views/defaultlayout.hpp"

#include <algorithm>

AnnounceView::AnnounceView(ViewStack* vs) : BaseView(vs) {
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
}

void AnnounceView::render_body(Graphics& g, render_box const& pos) {
  auto it = A11s::instance.msgs.rbegin();
  auto e = it;
  const size_t num_lines = pos.h;
  if (A11s::instance.msgs.size() < num_lines)
    e = A11s::instance.msgs.rend();
  else
    advance(e, num_lines);

  int r = num_lines;
  while (it != e) {
	  g.drawString(pos.x, pos.y + r - 1, *it,true, Graphics::colors_to_context[Graphics::WHITE]);
    --r;
    ++it;
  }
}
