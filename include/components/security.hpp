#pragma once
#include <cstdlib>
#include <cassert>

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
      ALL = (unsigned int)(-1)
  };

  inline Mask and_below(Mask m) {
    switch (m) {
    case INFRARED:
      return INFRARED;
    case RED:
      return (Mask)(RED & and_below(INFRARED));
    case ORANGE:
      return (Mask)(ORANGE & and_below(RED));
    case YELLOW:
      return (Mask)(YELLOW & and_below(ORANGE));
    case GREEN:
      return (Mask)(GREEN & and_below(YELLOW));
      // Got bored here.
    case ULTRAVIOLET:
      return (Mask)(ULTRAVIOLET & and_below(VIOLET));
    default:
      assert(false);
      return ALL;
    }
  }

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

