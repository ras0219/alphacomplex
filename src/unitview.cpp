#include "unitview.hpp"
#include "X11/keysym.h"
#include "viewstack.hpp"
#include "entity.hpp"
#include "citizen.hpp"

extern bool paused;

struct UnitListing : Component {
  virtual void render(struct Graphics& g);
} ulist;

void UnitListing::render(Graphics& g) {
  int col = 1;
  for (auto d : Department::List) {
    g.drawString(109 + col*24, 29, Department::mask_to_dcode(d), Graphics::DEFAULT);
    ++col;
  }


  int row = 1;
  char buf[60];

  for (auto e : AIEntity::ai_list) {
    e->description(buf, 60);
    g.drawString(5, 29+10*row, buf, Graphics::DEFAULT);

    col = 1;
    for (auto d : Department::List) {
      char buf2[] = { VBAR, ' ', ' ', ' ', 0 };
      if (e->rawname() == Citizen::RAWNAME && d & e->as<Citizen>().department())
        buf2[2] = 'X';
      g.drawString(100 + col*24, 29+10*row, buf2, Graphics::DEFAULT);
      ++col;
    }

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

