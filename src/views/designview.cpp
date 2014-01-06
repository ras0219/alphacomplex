#include "views/designview.hpp"

#include "views/viewstack.hpp"
#include "views/mapview.hpp"
#include "views/hud.hpp"
#include "views/statustext.hpp"

#include "entities/citizen.hpp"

extern bool paused;

void DesignView::render(Graphics& g) {
  mv.render(g);
  hud.render(g);
  //helptext.render(g);
  statustext.render(g);
}

void DesignView::handle_keypress(KeySym ks) {
  switch (ks) {
  case KEY_Escape:
    vstk->pop();
    break;
  case KEY_space:
    paused = !paused;
    break;
  case KEY_Left: return mv.left();
  case KEY_Right: return mv.right();
  case KEY_Up: return mv.up();
  case KEY_Down: return mv.down();
  case KEY_Return: return city.toggle_dig_wall(mv.csr.x, mv.csr.y);
  default:
    break;
  }
}
