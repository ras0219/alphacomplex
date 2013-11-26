#include "unitview.hpp"
#include "hud.hpp"
#include "X11/keysym.h"
#include "viewstack.hpp"

extern bool paused;

UnitListing ulist;

void UnitView::render(Graphics& g) {
  ulist.render(g);
}

void UnitView::handle_keypress(KeySym ks) {
  switch (ks) {
  case XK_Escape:
    vstk->pop();
    break;
  case XK_space:
    paused = !paused;
    break;
  default:
    break;
  }
}
