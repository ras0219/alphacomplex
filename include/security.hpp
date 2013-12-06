#pragma once

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

