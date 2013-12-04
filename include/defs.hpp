#pragma once

#include <tuple>
#include <array>
#include "debug_policy.hpp"

using namespace std;

typedef unsigned int uint;

typedef pair<int,int> point;
typedef tuple<int,int,int,int> waypoint;

enum Direction {
  EAST,
  SOUTH,
  WEST,
  NORTH
};

#include "security.hpp"
#include "department.hpp"
#include "faction.hpp"

typedef DebugPolicy<true> debug_policy_t;

void announce(const string&);

#include "keys.hpp"

extern bool paused;
