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

  inline const char* mask_to_local(Mask m) {
    switch (m) {
    case INFRARED:
      return "INFRARED";
    case RED:
      return "RED";
    case ORANGE:
      return "ORANGE";
    case YELLOW:
      return "YELLOW";
    case GREEN:
      return "GREEN";
      // Got bored here.
    case ULTRAVIOLET:
      return "ULTRAVIOLET";
    default:
      return "UNKNOWN";
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

  constexpr const array<Mask,5> List = {{
      INTERNAL_SECURITY,
      AGRICULTURE,
      FACILITIES,
      MECHANICULTURE,
      RESEARCH,
    }};

  inline const char* mask_to_dcode(Mask m) {
    switch (m) {
    case INTERNAL_SECURITY:
      return "IS";
    case AGRICULTURE:
      return "AG";
    case FACILITIES:
      return "FC";
    case MECHANICULTURE:
      return "MC";
    case RESEARCH:
      return "RS";
    default:
      return "??";
    }
  }

  inline const char* mask_to_local(Mask m) {
    switch (m) {
    case INTERNAL_SECURITY:
      return "INTERNAL SECURITY";
    case AGRICULTURE:
      return "AGRICULTURE";
    case FACILITIES:
      return "FACILITIES";
    case MECHANICULTURE:
      return "MECHANICULTURE";
    case RESEARCH:
      return "RESEARCH";
    default:
      return "??";
    }
  }
}

constexpr inline Department::Mask operator|(Department::Mask m1, Department::Mask m2) {
  return (Department::Mask)((uint)m1 | (uint)m2);
}
constexpr inline Security::Mask operator|(Security::Mask m1, Security::Mask m2) {
  return (Security::Mask)((uint)m1 | (uint)m2);
}

typedef DebugPolicy<true> debug_policy_t;

void announce(const string&);

#ifndef __MINGW32__
#include <X11/Xlib.h>
#include <X11/keysym.h>
#define KEY_Escape XK_Escape
#define KEY_space XK_space
#define KEY_Left XK_Left
#define KEY_Right XK_Right
#define KEY_Up XK_Up
#define KEY_Down XK_Down
#define KEY_Execute XK_Execute
#define KEY_KP_Enter XK_KP_Enter
#define KEY_Return XK_Return

#define KEY_h XK_h
#define KEY_u XK_u
#define KEY_r XK_r
#define KEY_a XK_a
#define KEY_q XK_q

#else

#include "SDL.h"

typedef unsigned long KeySym;

#define KEY_Escape SDLK_Escape
#define KEY_space SDLK_space
#define KEY_Left SDLK_Left
#define KEY_Right SDLK_Right
#define KEY_Up SDLK_Up
#define KEY_Down SDLK_Down
#define KEY_Execute SDLK_Execute
#define KEY_KP_Enter SDLK_KP_Enter
#define KEY_Return SDLK_Return

#define KEY_h SDLK_h
#define KEY_u SDLK_u
#define KEY_r SDLK_r
#define KEY_a SDLK_a
#define KEY_q SDLK_q

#endif

