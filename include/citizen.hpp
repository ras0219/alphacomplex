#pragma once

#include "entity.hpp"
#include "defs.hpp"
#include "joblist.hpp"
#include "skills.hpp"
#include "clearance.hpp"

Ent* make_new_red(int x, int y, City* c);

struct 



struct AIState;



struct Citizen : AIEntity {
  Citizen(int x_, int y_, Security::Mask s, City& c);

  // NOTE: calling this method will result in a suspend() message!
  int push_aistate(AIState*);

  Skill::Skillset skills;
  array<bool,Skill::NUM_SKILLS> skill_en;

  string name;
  string sect;
  Security::Mask sec;
  Faction::Mask fact;
  Department::Mask dept;
  int energy = 0;
  int clean_supplies = 0;

  friend struct AIState;
private: struct AIState* aistate;
public:

  unsigned int ssn;

  static const char* RAWNAME;
};
