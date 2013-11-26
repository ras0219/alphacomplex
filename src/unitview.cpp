#include "defs.hpp"
#include "unitview.hpp"
#include "viewstack.hpp"
#include "entity.hpp"
#include "citizen.hpp"

#include <algorithm>

extern bool paused;

struct UnitListing : Component {
  UnitListing() : row(0), col(0) { }

  virtual void render(struct Graphics& g);

  void down() {
    if (row+1 < AIEntity::ai_list.size())
      row++;
  }
  void right() {
    if (col+1 < Department::List.size())
      ++col;
  }
  void up() {
    if (row > 0)
      --row;
  }
  void left() {
    if (col > 0)
      --col;
  }

  bool check() {
    bool r = false;
    if (row >= AIEntity::ai_list.size()) {
      row = 1;
      r = true;
    }
    if (col >= Department::List.size()) {
      col = 1;
      r = true;
    }
    return r;
  }

  void toggle() {
    if (check()) return;

    auto it = AIEntity::ai_list.begin();
    advance(it, row);

    if ((*it)->rawname() != Citizen::RAWNAME) {
      announce("This unit is not a duty-assignable citizen.");
      return;
    }

    Citizen& c = (*it)->as<Citizen>();
    c.dept = (Department::Mask)(c.dept ^ Department::List[col]);
  }

  uint row, col;
} ulist;

void UnitListing::render(Graphics& g) {
  g.drawString(5, 17, "Citizen Roster", Graphics::DEFAULT);

  int c = 0;
  for (auto d : Department::List) {
    g.drawString(133 + c*24, 29, Department::mask_to_dcode(d), Graphics::DEFAULT);
    ++c;
  }

  int r = 0;
  char buf[60];

  for (auto e : AIEntity::ai_list) {
    e->description(buf, 18);
    g.drawString(22, 41+12*r, buf, Graphics::DEFAULT);

    c = 0;
    for (auto d : Department::List) {
      char buf2[] = { VBAR, ' ', ' ', ' ', 0 };
      if (c == col && r == row) {
        buf2[1] = '<';
        buf2[3] = '>';
      }
      if (e->rawname() == Citizen::RAWNAME && d & e->as<Citizen>().department())
        buf2[2] = 'X';
      g.drawString(124 + c*24, 41+12*r, buf2, Graphics::DEFAULT);
      ++c;
    }

    ++r;
  }

  // Render cursor
  g.drawChar(5, 39 + 12*row, '>', Graphics::DEFAULT);
  g.drawChar(136 + 24*col, 17, 'V', Graphics::DEFAULT);
}

void UnitView::render(Graphics& g) {
  ulist.render(g);
}

void UnitView::handle_keypress(KeySym ks) {
  switch (ks) {
  case KEY_Escape:
    vstk->pop();
    break;
  case KEY_space:
    paused = !paused;
    break;
  case KEY_Left: return ulist.left();
  case KEY_Right: return ulist.right();
  case KEY_Up: return ulist.up();
  case KEY_Down: return ulist.down();

  case KEY_Execute:
  case KEY_KP_Enter:
  case KEY_Return: return ulist.toggle();

  default: return; // maybe play an alert here?
  }
}

