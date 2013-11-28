#include "defs.hpp"
#include "announceview.hpp"
#include "viewstack.hpp"
#include "hud.hpp"
#include "graphics.hpp"

#include <algorithm>

void AnnounceView::render(Graphics& g) {
  auto it = a11s.msgs.rbegin();
  auto e = it;
  if (a11s.msgs.size() < 40)
    e = a11s.msgs.rend();
  else
    advance(e, 40);

  int r = 40;
  while (it != e) {
    g.drawString(5, 5+12+12*r, *it, Graphics::DEFAULT);
    --r;
    ++it;
  }
}

void AnnounceView::handle_keypress(KeySym ks) {
  switch (ks) {
  case KEY_Escape:
    vstk->pop();
    break;
  case KEY_space:
    paused = !paused;
    break;
  // case KEY_Left: return ulist.left();
  // case KEY_Right: return ulist.right();
  // case KEY_Up: return ulist.up();
  // case KEY_Down: return ulist.down();

  // case KEY_Return: return ulist.toggle();

  default: return; // maybe play an alert here?
  }
}

