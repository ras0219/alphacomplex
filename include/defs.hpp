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

#include "components/security.hpp"
#include "components/department.hpp"
#include "components/faction.hpp"

typedef DebugPolicy<true> debug_policy_t;

void announce(const string&);

#include "keys.hpp"

extern bool paused;

// These definitions determine the desired framerates
// units are microseconds per frame (gfx=graphics, lgc=logic)

// Example config: (30 graphical fps, 100 logical fps)
// #define US_PER_GFX 33000
// #define US_PER_LGC 10000

#define US_PER_GFX 33000
#define US_PER_LGC 10000
#define US_PER_REP 1000000

