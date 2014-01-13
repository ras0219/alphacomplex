#include "views/unitviews/unitviewmode.hpp"
#include "components/skills.hpp"

struct SkillsMode : UnitViewModeInstance<SkillsMode> {
  static inline string title() { return "Units Roster (Skills)"; }
  static inline const Skill::List_t& col_list() { return Skill::List; }
  static inline string col_label(Skill::Category& d) { return Skill::shortname(d); }
  static inline void entry(CitizenName* cn, Skill::Category, string& buf) { (void)cn; (void)buf; }

  virtual uint num_cols() const override { return col_list().size(); }
  virtual void toggle(CitizenName::iterator, uint) override { }
};

SkillsMode skillsmode;
