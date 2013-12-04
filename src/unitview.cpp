#include "defs.hpp"
#include "unitview.hpp"
#include "viewstack.hpp"
#include "entity.hpp"
#include "citizen.hpp"
#include "hud.hpp"

#include <algorithm>

extern bool paused;

struct UnitListing : Component {
  UnitListing() : row(0), col(0), assign_mode(true) { }

  virtual void render(struct Graphics& g);
  void render_skills(Graphics&);
  void render_assign(Graphics&);

  void down() {
    if (row+1 < AIEntity::ai_list.size())
      row++;
  }
  void right() {
    if (assign_mode) {
      if (col+1 < Department::List.size())
        ++col;
    } else {
      if (col+1 < Skill::NUM_SKILLS)
        ++col;
    }
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
      row = AIEntity::ai_list.size() - 1;
      r = true;
    }
    if (assign_mode && col >= Department::List.size()) {
      col = Department::List.size() - 1;
      r = true;
    }
    if (!assign_mode && col >= Skill::NUM_SKILLS) {
      col = Skill::NUM_SKILLS - 1;
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
    if (c.faction() != Faction::PLAYER) {
      announce("This unit is not under your control.");
      return;
    }

    if (assign_mode)
      c.dept = Department::List[col];
    else
      c.skill_en[col] = !c.skill_en[col];
  }

  void mode_toggle() { assign_mode = !assign_mode; check(); }

  uint row, col;
  bool assign_mode;
} ulist;

struct Spreadsheet {
  string name;
  uint row_off;
  uint col_off;
  uint left;
};

void UnitListing::render(Graphics& g) {
  if (assign_mode)
    render_assign(g);
  else
    render_skills(g);
}

void UnitListing::render_assign(Graphics& g) {
  uint row_off = 12;
  uint col_off = 42;
  uint left = 124;
  uint top_row = 42;

  g.drawString(6, 18, "Units Roster (Dept)", Graphics::DEFAULT);

  uint c = 0;
  for (auto d : Department::List) {
    g.drawString(left + 6 + c*col_off, top_row,
                 Department::mask_to_local6(d), Graphics::DEFAULT);
    ++c;
  }

  uint r = 0;
  char buf[60];

  for (auto e : AIEntity::ai_list) {
    e->description(buf, 18);
    g.drawString(18, top_row + 12 + r*row_off, buf, Graphics::DEFAULT);

    c = 0;
    for (auto d : Department::List) {
      array<char,8> buf2 = {{ VBAR, ' ', ' ', ' ', ' ', ' ', ' ', 0 }};
      if (c == col && r == row) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      if (e->rawname() == Citizen::RAWNAME) {
        Citizen& cit = e->as<Citizen>();
        if (d & cit.department()) {
          buf2[3] = 'X';
          buf2[4] = 'X';
        }
      }
      g.drawString(left + c*col_off, top_row + 12 + r*row_off, buf2.begin(), Graphics::DEFAULT);
      ++c;
    }

    ++r;
  }

  // Render cursor
  g.drawChar(6, top_row + 12 + row*row_off, '>', Graphics::DEFAULT);
  g.drawString(left + 18 + col*col_off, 18, "VV", Graphics::DEFAULT);
}

void UnitListing::render_skills(Graphics& g) {
  uint row_off = 12;
  uint col_off = 42;
  uint left = 124;
  uint top_row = 42;

  g.drawString(6, 18, "Units Roster (Skills)", Graphics::DEFAULT);

  uint c = 0;
  for (auto d : Skill::List) {
    g.drawString(left + 6 + c*col_off, 30 + (c % 2 ? 0 : 12),
                 Skill::shortname(d), Graphics::DEFAULT);
    ++c;
  }

  uint r = 0;
  char buf[60];

  for (auto e : AIEntity::ai_list) {
    e->description(buf, 18);
    g.drawString(18, top_row + 12 + r*row_off, buf, Graphics::DEFAULT);

    c = 0;
    for (auto d : Skill::List) {
      array<char,8> buf2 = {{ VBAR, ' ', ' ', ' ', ' ', ' ', ' ', 0 }};
      if (c == col && r == row) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      if (e->rawname() == Citizen::RAWNAME) {
        Citizen& cit = e->as<Citizen>();
        int lv = cit.skills[d].lv();
        buf2[2] = '0' + (lv / 10);
        buf2[3] = '0' + (lv % 10);
        if (cit.skill_en[d]) {
          buf2[5] = 'X';
        }
      }
      g.drawString(left + c*col_off, top_row + 12 + r*row_off, buf2.begin(), Graphics::DEFAULT);
      ++c;
    }

    ++r;
  }

  // Render cursor
  g.drawChar(6, top_row + 12 + row*row_off, '>', Graphics::DEFAULT);
  g.drawString(left + 18 + col*col_off, 18, "VV", Graphics::DEFAULT);
}

void UnitView::render(Graphics& g) {
  ulist.render(g);
  hud.render(g);
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

  case KEY_Return: return ulist.toggle();

  case KEY_Tab: return ulist.mode_toggle();

  default: return; // maybe play an alert here?
  }
}

