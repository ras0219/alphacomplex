#pragma once

#include "point.hpp"
#include "ai.hpp"

AI::script_ptr make_do_at(point destination, int duration, std::string description, int walkrate = 5);
