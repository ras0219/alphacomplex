#pragma once

#include "entity.hpp"
#include "defs.hpp"
#include "joblist.hpp"
#include "components/skills.hpp"
#include "components/clearance.hpp"

string get_full_name(Ent*);

Ent* new_citizen(Position p, Security::Mask s = Security::RED);
