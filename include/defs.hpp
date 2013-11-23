#pragma once

#include <tuple>
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

typedef DebugPolicy<true> debug_policy_t;
