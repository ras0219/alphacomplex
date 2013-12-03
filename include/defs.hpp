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

  const array<Mask,5> List = {{
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

namespace Faction {
  enum Mask : unsigned int {
    PLAYER = 1,
      COMPUTER = 2,
      HOSTILE = 4,
      PASSIVE = 8,
      ALL = (uint)(-1)
  };

  const array<Mask,5> List = {{
      PLAYER,
      COMPUTER,
      HOSTILE,
      PASSIVE
    }};

  inline const char* mask_to_dcode(Mask m) {
    switch (m) {
    case PLAYER:
      return "PL";
    case COMPUTER:
      return "CP";
    case HOSTILE:
      return "HS";
    case PASSIVE:
      return "PS";
    default:
      return "??";
    }
  }

  inline const char* mask_to_local(Mask m) {
    switch (m) {
    case PLAYER:
      return "PLAYER";
    case COMPUTER:
      return "COMPUTER";
    case HOSTILE:
      return "HOSTILE";
    case PASSIVE:
      return "PASSIVE";
    default:
      return "??";
    }
  }
}

typedef DebugPolicy<true> debug_policy_t;

void announce(const string&);

#ifdef GRAPHICS_X11
#include <X11/Xlib.h>
#include <X11/keysym.h>
#define KEY_Escape XK_Escape
#define KEY_space XK_space
#define KEY_Left XK_Left
#define KEY_Right XK_Right
#define KEY_Up XK_Up
#define KEY_Down XK_Down
//#define KEY_KP_Enter XK_KP_Enter
#define KEY_Return XK_Return

#define KEY_h XK_h
#define KEY_u XK_u
#define KEY_r XK_r
#define KEY_a XK_a
#define KEY_q XK_q
#define KEY_e XK_e
#define KEY_d XK_d

#else
#include "SDL.h"

typedef unsigned long KeySym;

#define KEY_Escape SDLK_ESCAPE
#define KEY_space SDLK_SPACE
#define KEY_Left SDLK_LEFT
#define KEY_Right SDLK_RIGHT
#define KEY_Up SDLK_UP
#define KEY_Down SDLK_DOWN
#define KEY_Return SDLK_RETURN

#define KEY_h SDLK_h
#define KEY_u SDLK_u
#define KEY_r SDLK_r
#define KEY_a SDLK_a
#define KEY_q SDLK_q
#define KEY_e SDLK_e
#define KEY_d SDLK_d

#endif

extern bool paused;
