#pragma once

#include "defs.hpp"
#include <cassert>

namespace Skill {
  inline int xp_of_level(int lv) {
    return lv * 1000;
  }
  inline int level_of_xp(int xp) {
    return xp / 1000;
  }

  struct Skill {
    Skill() {}
    Skill(int level) : xp_(xp_of_level(level)) { }

    int xp_;

    inline void add_xp(int x) {
      xp_ += x;
      if (xp_ > 1000)
        xp_ = 1000;
    }
    inline int xp() { return xp_; }
    inline int lv() { return level_of_xp(xp_); }
  };

  enum Category {
    // Technical Skills
    BOT_MAINTENANCE,
    BOT_OPERATION,
    BOT_PROGRAMMING,
    REACTOR_MAINTENANCE,
    REACTOR_OPERATION,
    REACTOR_PROGRAMMING,
    VEHICLE_MAINTENANCE,
    VEHICLE_OPERATION,
    VEHICLE_PROGRAMMING,

    // Science Skills
    REACTOR_DESIGN,
    PHARMACEUTICALS,

    // Physical Skills
    DRUG_ADMINISTRATION,
    CONSTRUCTION,
    MINING_LASER,

    // Management Skills
    BUREAUCRACY,

    NUM_SKILLS
  };

  typedef array<Category,NUM_SKILLS> List_t;
  const List_t List = {{
      // Technical Skills
      BOT_MAINTENANCE,
      BOT_OPERATION,
      BOT_PROGRAMMING,
      REACTOR_MAINTENANCE,
      REACTOR_OPERATION,
      REACTOR_PROGRAMMING,
      VEHICLE_MAINTENANCE,
      VEHICLE_OPERATION,
      VEHICLE_PROGRAMMING,

      // Science Skills
      REACTOR_DESIGN,
      PHARMACEUTICALS,

      // Physical Skills
      DRUG_ADMINISTRATION,
      CONSTRUCTION,
      MINING_LASER,

      // Management Skills
      BUREAUCRACY
    }};

  struct Skillset {
    array<Skill,NUM_SKILLS> skills;

    inline Skill& get(Category c) {
      assert(c >= 0 && c < NUM_SKILLS);
      return skills[c];
    }
    inline const Skill& get(Category c) const {
      assert(c >= 0 && c < NUM_SKILLS);
      return skills[c];
    }

    inline Skill& operator[](int c) { return get((Category)c); }
    inline const Skill& operator[](int c) const { return get((Category)c); }
  };

  const char* shortname(Category c);
  const char* skillname(Category c);
  
  Skillset random_skills(Security::Mask);
}

