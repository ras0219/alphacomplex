#pragma once

#include <tuple>
#include <array>
#include "debug_policy.hpp"

using namespace std;

typedef unsigned int uint;

#include "point.hpp"
typedef tuple<int,int,int,int> waypoint;

/// Use to describe absolute directions
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

/// Globally accessible function to send an announcement to the announcement singleton
void announce(const std::string&);

#include "keys.hpp"

/// Globally accessible simulation state for whether the game logic should be run
extern bool paused;

// These definitions determine the desired framerates
// units are microseconds per frame (gfx=graphics, lgc=logic)

// Example config: (30 graphical fps, 100 logical fps)
// #define US_PER_GFX 33000
// #define US_PER_LGC 10000
#define US_PER_GFX 33000
#define US_PER_LGC 10000

// This defines the rate of FPS reporting into the announcements; recommended is 1000000 or 1 report per second
#define US_PER_REP 1000000
