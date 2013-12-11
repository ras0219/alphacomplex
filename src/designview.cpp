#include "designview.hpp"

#include "viewstack.hpp"
#include "mapview.hpp"
#include "hud.hpp"
#include "citizen.hpp"
#include "statustext.hpp"

extern bool paused;

DesignView::DesignView(ViewStack* vs, MapView* m, City& c)
  : vstk(vs), mv(m), city(c) { }

void DesignView::render(Graphics& g) {
  mv->csr_enable = true;
  mv->render(g);
  hud.render(g);
  //helptext.render(g);
  statustext.render(g);
}

void DesignView::handle_keypress(KeySym ks) {
  switch (ks) {
  case KEY_Escape:
    mv->csr_enable = false;
    vstk->pop();
    break;
  case KEY_space:
    paused = !paused;
    break;
  case KEY_Left: return mv->left();
  case KEY_Right: return mv->right();
  case KEY_Up: return mv->up();
  case KEY_Down: return mv->down();

  case KEY_Return: return mv->zap_wall();
  default:
    break;
  }
}
