#include "views/unitviews/unitviewmode.hpp"
#include "needs/needssystem.hpp"

using namespace ecs;
using namespace needs;

struct NeedsMode : UnitViewModeInstance<NeedsMode> {
  static inline string title() { return "Units Roster (Needs)"; }
  using List_t = std::array<std::string, 3>;
  static List_t List;

  static inline const List_t& col_list() { return List; }
  static inline const string& col_label(const std::string& d) { return d; }
  static inline void entry(CitizenName* cn, const std::string& l, string& buf) {
    Ent* e = cn->parent;
    if (!e->has<Needs>()) {
      buf = "N/A";
      return;
    }
    Needs* nai = e->get<Needs>();
    int val;
    if (l == "Food") {
      val = nai->food;
    } else if (l == "Sleep") {
      val = nai->sleep;
    } else if (l == "Happy") {
      val = nai->happy;
    } else {
      assert(false);
      exit(0);
    }
    buf[5] = '0' + (val % 10);
    buf[4] = '0' + ((val / 10) % 10);
    buf[3] = '0' + ((val / 100) % 10);
  }

  virtual uint num_cols() const override { return col_list().size(); }
  virtual void toggle(CitizenName::iterator, uint) override { }
};

NeedsMode::List_t NeedsMode::List = {{ "Food", "Sleep", "Happy" }};

NeedsMode needsmode;
