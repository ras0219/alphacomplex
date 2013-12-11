#pragma once

#include "entity.hpp"
#include "defs.hpp"
#include "joblist.hpp"
#include "skills.hpp"
#include "clearance.hpp"

string get_full_name(Ent*);

Ent* new_citizen(Position p, Security::Mask s = Security::RED);
