#include "unitview.hpp"
#include "X11/keysym.h"
#include "viewstack.hpp"
#include "entity.hpp"

extern bool paused;

struct UnitListing : Component {
  virtual void render(struct Graphics& g);
} ulist;

void UnitListing::render(Graphics& g) {
  int row = 1;
  char buf[60];
  for (auto e : AIEntity::ai_list) {
    e->description(buf, 60);
    g.drawString(5, 5+10*row, buf, Graphics::DEFAULT);
    ++row;
  }
}

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

