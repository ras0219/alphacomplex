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
#include "views/scrollable.hpp"
#include "views/defaultlayout.hpp"

#include <memory>
#include <algorithm>

extern bool paused;

struct UnitListing : StaticWidget<UnitListing> {
  UnitListing() : csr_row(0), csr_col(0), mode(modes.begin()) { }

  void render(Graphics& g, render_box const& pos) { return getmode().render(g, pos, csr_row, csr_col); }

  inline void down() {
    if (csr_row + 1 < CitizenName::instances.size())
      csr_row++;
  }
  inline void right() {
    if (csr_col + 1 < getmode().num_cols())
      ++csr_col;
  }
  inline void up() {
    if (csr_row > 0)
      --csr_row;
  }
  inline void left() {
    if (csr_col > 0)
      --csr_col;
  }

  inline uint num_rows() const { return CitizenName::instances.size(); }

  bool check();
  inline UnitViewMode& getmode() { return **mode; }
  void toggle();
  void mode_switch() { ++mode; if (mode == modes.end()) mode = modes.begin(); check(); }

  uint csr_row, csr_col;

  using Mode_t = UnitViewMode*;
  using Modes_t = std::array<Mode_t, 3>;
  static Modes_t modes;
  Modes_t::iterator mode;
} ulist;

extern struct AssignMode assignmode;
extern struct SkillsMode skillsmode;
extern struct NeedsMode needsmode;

UnitListing::Modes_t UnitListing::modes = { {
  (UnitListing::Mode_t) &assignmode,
  (UnitListing::Mode_t) &needsmode,
  (UnitListing::Mode_t) &skillsmode
} };

bool UnitListing::check() {
  bool r = false;
  if (csr_row >= CitizenName::instances.size()) {
    csr_row = CitizenName::instances.size() - 1;
    r = true;
  }
  uint x = getmode().num_cols();

  if (csr_col >= x) {
    csr_col = x - 1;
    r = true;
  }
  return r;
}

void UnitListing::toggle() {
  if (check()) return;

  auto it = CitizenName::instances.begin();
  advance(it, csr_row);

  // announce("This unit is not a duty-assignable citizen.");
  // return;

  getmode().toggle(it, csr_col);
}

UnitView::UnitView(ViewStack* vs) : vstk(vs) {
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
  nav.register_key(KEY_Return, "[Ent] Toggle", [this]() { ulist.toggle(); });
  nav.register_key(KEY_Tab, "[Tab] Mode", [this]() { ulist.mode_switch(); });
}

void UnitView::render(Graphics& g, render_box const& pos) {
  render_box pos2 = DefaultLayout::render_layout(this, g, pos);

  ulist.render(g, pos2);
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

