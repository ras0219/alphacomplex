#pragma once

#include "entity.hpp"
#include "defs.hpp"
#include "joblist.hpp"
#include "skills.hpp"

#include <string>
#include <vector>

using std::vector;
using std::string;

struct Citizen : AIEntity {
  Citizen(int x_, int y_, Security::Mask s, City& c);

  virtual const char* rawname() const { return RAWNAME; }
  virtual int description(char* buf, size_t n) const;

  virtual char render() const;
  virtual void update();

  Security::Mask security() const;
  inline Department::Mask department() const { return dept; }
  inline Faction::Mask faction() const { return fact; }

  void interrupt(struct Job*);
  void resume();
  void finalize_job();
  void path_to(int, int);
  void set_job(struct Job*);

  Skill::Skillset skills;
  array<bool,Skill::NUM_SKILLS> skill_en;

  string name;
  string sect;
  Security::Mask sec;
  Faction::Mask fact;
  Department::Mask dept;
  int energy = 0;
  int clean_supplies = 0;


  void update_idle();
  void update_walkingtojob();
  void update_wandering();
  void update_activity();
  void update_sleeping();

  inline void set_idle() { energy = -10; state = IDLE; }
  inline void set_sleeping() { energy = -100; state = SLEEPING; }
  inline void set_walkingtojob() { energy = -2; state = WALKINGTOJOB; }
  inline void set_wandering() { energy = -5; state = WANDERING; }
  inline void set_activity(int i) { energy = -i; state = ACTIVITY; }

  enum StateMachine {
    IDLE,
    WALKINGTOJOB,
    WANDERING,
    ACTIVITY,
    SLEEPING
  };
private: StateMachine state;
public:

  Job* job;
  JobList::iterator job_it;
  vector<Job*> suspended_jobs;

  vector<point> path;
  vector<point>::reverse_iterator pathp;

  Job* intsec_review_job;

  unsigned int ssn;

  static const char* RAWNAME;
};
