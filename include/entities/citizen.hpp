#pragma once

//#include "entity.hpp"
#include "components/security.hpp"
#include "point.hpp"
//#include "joblist.hpp"
//#include "components/skills.hpp"
//#include "components/clearance.hpp"

#include <string>

std::string get_full_name(struct Ent*);

struct Ent* new_citizen(Point p, Security::Mask s = Security::RED);
