#include "defs.hpp"
#include "views/unitview.hpp"
#include "views/viewstack.hpp"
#include "entities/entity.hpp"
#include "entities/citizen.hpp"
#include "views/hud.hpp"
#include "windows.hpp"
#include "components/citizenname.hpp"
#include "views/statustext.hpp"
#include "views/unitviews/unitviewmode.hpp"

#include <memory>
#include <algorithm>

extern bool paused;


struct UnitListing : Widget {
  UnitListing() : row(0), col(0), mode(modes.begin()) { }
  virtual void render(Graphics& g) {
    getmode().render(g, row, col);
  }

  inline void down() {
    if (row + 1 < rows())
      row++;
  }
  inline void right() {
    if (col + 1 < getmode().num_cols())
      ++col;
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

  bool check();
  inline UnitViewMode& getmode() { return **mode; }
  void toggle();
  void mode_switch() { ++mode; if (mode == modes.end()) mode = modes.begin(); check(); }

  uint row, col;
  using Mode_t = UnitViewMode*;
  using Modes_t = std::array<Mode_t, 3>;
  static Modes_t modes;
  Modes_t::iterator mode;
} ulist;

extern struct AssignMode assignmode;
extern struct SkillsMode skillsmode;
extern struct NeedsMode needsmode;

UnitListing::Modes_t UnitListing::modes = {
  (UnitListing::Mode_t) &assignmode,
  (UnitListing::Mode_t) &needsmode,
  (UnitListing::Mode_t) &skillsmode
};

#include <sstream>

bool UnitListing::check() {
  bool r = false;
  if (row >= rows()) {
    row = rows() - 1;
    r = true;
  }
  uint x = getmode().num_cols();

  if (col >= x) {
    col = x - 1;
    r = true;
  }
  return r;
}

void UnitListing::toggle() {
  if (check()) return;

  auto it = CitizenName::instances.begin();
  advance(it, row);

  // announce("This unit is not a duty-assignable citizen.");
  // return;

  getmode().toggle(it, col);
}

UnitView::UnitView(ViewStack* vs) : vstk(vs) {
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
  nav.register_key(KEY_Return, "[Ent] Toggle", [this]() { ulist.toggle(); });
  nav.register_key(KEY_Tab, "[Tab] Mode", [this]() { ulist.mode_switch(); });
}

void UnitView::render(Graphics& g) {
  ulist.render(g);
  hud.render(g);
  statustext.render(g);
  nav.render(g);
}

void UnitView::handle_keypress(KeySym ks) {
  if (nav.handle_keypress(ks)) return;

  switch (ks) {
  case KEY_Left: return ulist.left();
  case KEY_Right: return ulist.right();
  case KEY_Up: return ulist.up();
  case KEY_Down: return ulist.down();

  default: return; // maybe play an alert here?
  }
}

