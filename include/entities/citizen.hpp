#pragma once

#include "components/security.hpp"
#include "point.hpp"

#include <string>

namespace ecs { struct Ent; }

std::string get_full_name(ecs::Ent*);

ecs::Ent* new_citizen(Point p, Security::Mask s = Security::RED);
