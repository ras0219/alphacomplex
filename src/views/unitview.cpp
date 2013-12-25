#include "defs.hpp"
#include "views/unitview.hpp"
#include "views/viewstack.hpp"
#include "entities/entity.hpp"
#include "entities/citizen.hpp"
#include "views/hud.hpp"
#include "windows.hpp"
#include "components/citizenname.hpp"
#include "views/statustext.hpp"

#include <algorithm>

extern bool paused;

struct UnitListing : Widget {
  UnitListing() : row(0), col(0), assign_mode(true) { }

  virtual void render(Graphics& g);

  template<class Policy>
  void render_policy(Graphics&);

  inline void down() {
    if (row+1 < rows())
      row++;
  }
  inline void right() {
    if (assign_mode) {
      if (col+1 < Department::List.size())
        ++col;
    } else {
      if (col+1 < Skill::NUM_SKILLS)
        ++col;
    }
  }
  inline void up() {
    if (row > 0)
      --row;
  }
  inline void left() {
    if (col > 0)
      --col;
  }

  inline uint rows() const { return CitizenName::instances.size(); }

  bool check() {
    bool r = false;
    if (row >= rows()) {
      row = rows() - 1;
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

  void toggle();

  void mode_toggle() { assign_mode = !assign_mode; check(); }

  uint row, col;
  bool assign_mode;
} ulist;

#include <sstream>

struct AssignPolicy {
  static inline string title() { return "Units Roster (Dept)"; }
  static inline const Department::List_t& col_list() { return Department::List; }
  static inline string col_label(Department::Mask& d) { return Department::mask_to_local6(d); }
  static inline void entry(CitizenName* cn, Department::Mask m, string& buf) {
    ClearanceComp* cc = cn->parent->assert_get<ClearanceComp>();
    if (!(cc->department() & m))
      return;
    buf[3] = 'X';
    buf[4] = 'X';
  }

  static inline void toggle(CitizenName::iterator it, int col_num) {
    ClearanceComp* cc = (*it)->parent->assert_get<ClearanceComp>();
    cc->department() = Department::List[col_num];
  }
};
struct SkillsPolicy {
  static inline string title() { return "Units Roster (Skills)"; }
  static inline const Skill::List_t& col_list() { return Skill::List; }
  static inline string col_label(Skill::Category& d) { return Skill::shortname(d); }
  static inline void entry(CitizenName* cn, Skill::Category, string& buf) {(void)cn; (void)buf; }

  static inline void toggle(CitizenName::iterator it, int col_num) {(void)it;(void)col_num; }
};
void UnitListing::toggle() {
  if (check()) return;

  auto it = CitizenName::instances.begin();
  advance(it, row);

  // announce("This unit is not a duty-assignable citizen.");
  // return;

  if (assign_mode)
    AssignPolicy::toggle(it, col);
  else
    SkillsPolicy::toggle(it, col);
}

void UnitListing::render(Graphics& g) {
  if (assign_mode)
    render_policy<AssignPolicy>(g);
  else
    render_policy<SkillsPolicy>(g);
}

template<class P>
void UnitListing::render_policy(Graphics& g) {
  uint row_off = 12;
  uint col_off = 42;
  uint left = 124;
  uint top_row = 42;

  g.drawString(6, 18, P::title());

  uint c = 0;
  for (auto d : P::col_list()) {
    g.drawString(left + 6 + c*col_off, top_row, P::col_label(d));
    ++c;
  }

  uint r = 0;

  for (auto e : CitizenName::instances) {
    g.drawString(18, top_row + 12 + r*row_off, get_full_name(e->parent));

    c = 0;
    for (auto d : P::col_list()) {
      string buf2 = "|      ";
      if (c == col && r == row) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      P::entry(e, d, buf2);

      g.drawString(left + c*col_off, top_row + 12 + r*row_off, buf2);
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
  statustext.render(g);
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

