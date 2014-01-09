#include "views/unitviews/unitviewmode.hpp"
#include "components/skills.hpp"

struct SkillsMode : UnitViewMode {
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

SkillsMode skillsmode;
