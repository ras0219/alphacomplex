#pragma once

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
