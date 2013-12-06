#pragma once
#include <cstdlib>

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

  inline Mask random_dept() {
    return List[rand() % List.size()];
  }

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

  inline const char* mask_to_local6(Mask m) {
    switch (m) {
    case INTERNAL_SECURITY:
      return "INTSEC";
    case AGRICULTURE:
      return "AGRICL";
    case FACILITIES:
      return "FACILT";
    case MECHANICULTURE:
      return "MECHAN";
    case RESEARCH:
      return "RESRCH";
    default:
      return "??????";
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
