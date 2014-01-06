#include "defs.hpp"
#include "views/unitview.hpp"
#include "views/viewstack.hpp"
#include "entities/entity.hpp"
#include "entities/citizen.hpp"
#include "views/hud.hpp"
#include "windows.hpp"
#include "components/citizenname.hpp"
#include "views/statustext.hpp"
#include "components/ai/needsai.hpp"

#include <memory>
#include <algorithm>

extern bool paused;

struct UnitListingMode {
  virtual void render(Graphics& g, uint row, uint col) = 0;
  virtual uint num_cols() = 0;
  virtual void toggle(CitizenName::iterator it, uint col_num) = 0;
};

template<class P>
void render_policy(Graphics& g, uint row, uint col) {
  uint row_off = 12;
  uint col_off = 48;
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

struct AssignMode : UnitListingMode {
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

  void render(Graphics& g, uint row, uint col) {
    render_policy<AssignMode>(g, row, col);
  }
  uint num_cols() { return col_list().size(); }
  void toggle(CitizenName::iterator it, uint col_num) {
    ClearanceComp* cc = (*it)->parent->assert_get<ClearanceComp>();
    cc->department() = Department::List[col_num];
  }
};

struct SkillsMode : UnitListingMode {
  static inline string title() { return "Units Roster (Skills)"; }
  static inline const Skill::List_t& col_list() { return Skill::List; }
  static inline string col_label(Skill::Category& d) { return Skill::shortname(d); }
  static inline void entry(CitizenName* cn, Skill::Category, string& buf) { (void)cn; (void)buf; }


  void render(Graphics& g, uint row, uint col) {
    render_policy<SkillsMode>(g, row, col);
  }
  uint num_cols() { return col_list().size(); }
  void toggle(CitizenName::iterator, uint) { }
};

struct NeedsMode : UnitListingMode {
  static inline string title() { return "Units Roster (Needs)"; }
  using List_t = std::array<std::string, 3>;
  static List_t List;

  static inline const List_t& col_list() { return List; }
  static inline const string& col_label(const std::string& d) { return d; }
  static inline void entry(CitizenName* cn, const std::string& l, string& buf) {
    Ent* e = cn->parent;
    if (!e->has<NeedsAI>()) {
      buf = "N/A";
      return;
    }
    NeedsAI* nai = e->get<NeedsAI>();
    int val;
    if (l == "Food") {
      val = nai->food;
    } else if (l == "Sleep") {
      val = nai->sleep;
    } else if (l == "Happy") {
      val = nai->happy;
    } else {
      assert(false);
    }
    buf[5] = '0' + (val % 10);
    buf[4] = '0' + ((val / 10) % 10);
    buf[3] = '0' + ((val / 100) % 10);
  }

  void render(Graphics& g, uint row, uint col) {
    render_policy<NeedsMode>(g, row, col);
  }
  uint num_cols() { return col_list().size(); }
  void toggle(CitizenName::iterator, uint) { }
};

NeedsMode::List_t NeedsMode::List = { "Food", "Sleep", "Happy" };

struct UnitListing : Widget {
  UnitListing() : row(0), col(0), mode(modes.begin()) { }
  virtual void render(Graphics& g) {
    mode->get()->render(g, row, col);
  }

  inline void down() {
    if (row + 1 < rows())
      row++;
  }
  inline void right() {
    if (col + 1 < mode->get()->num_cols())
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

  void toggle();
  void mode_switch() { ++mode; if (mode == modes.end()) mode = modes.begin(); check(); }

  uint row, col;
  using Mode_t = std::unique_ptr<UnitListingMode>;
  using Modes_t = std::array<Mode_t, 3>;
  static Modes_t modes;
  Modes_t::iterator mode;
} ulist;

UnitListing::Modes_t UnitListing::modes = {
  std::make_unique<AssignMode>(),
  std::make_unique<SkillsMode>(),
  std::make_unique<NeedsMode>()
};

#include <sstream>

bool UnitListing::check() {
  bool r = false;
  if (row >= rows()) {
    row = rows() - 1;
    r = true;
  }
  uint x = mode->get()->num_cols();

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

  mode->get()->toggle(it, col);
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
  case KEY_Tab: return ulist.mode_switch();

  default: return; // maybe play an alert here?
  }
}

