#include "views/unitviews/unitviewmode.hpp"
#include "components/clearance.hpp"

struct AssignMode : UnitViewModeInstance<AssignMode> {
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

  virtual uint num_cols() const override { return col_list().size(); }
  virtual void toggle(CitizenName::iterator it, uint col_num) override {
    ClearanceComp* cc = (*it)->parent->assert_get<ClearanceComp>();
    cc->department() = Department::List[col_num];
  }
};

AssignMode assignmode;
