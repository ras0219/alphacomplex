#pragma once

#include <tuple>

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

namespace Security {
  enum Mask : unsigned int {
    INFRARED = 1,
      RED = 2,
      ORANGE = 4,
      YELLOW = 8,
      GREEN = 16,
      BLUE = 32,
      INDIGO = 64,
      VIOLET = 128,
      ULTRAVIOLET = 256,
      GAMMA = 512,
      ALL = (uint)(-1)
  };

  inline const char* mask_to_dcode(Mask m) {
    switch (m) {
    case INFRARED:
      return "IR";
    case RED:
      return "RD";
    case ORANGE:
      return "OR";
    case YELLOW:
      return "YL";
    case GREEN:
      return "GN";
      // Got bored here.
    case ULTRAVIOLET:
      return "UV";
    default:
      return "??";
    }
  }
}

namespace Department {
  enum Mask : unsigned int {
    INTERNAL_SECURITY = 1,
      AGRICULTURE = 2,
      FACILITIES = 4,
      MECHANICULTURE = 8,
      RESEARCH = 16,
      ALL = (uint)(-1)
  };
}

constexpr inline Department::Mask operator|(Department::Mask m1, Department::Mask m2) {
  return (Department::Mask)((uint)m1 | (uint)m2);
}
constexpr inline Security::Mask operator|(Security::Mask m1, Security::Mask m2) {
  return (Security::Mask)((uint)m1 | (uint)m2);
}
