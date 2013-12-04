#include "skills.hpp"

namespace Skill {
  Skillset random_skills(Security::Mask s) {
    // For now, ignore the clearance

    Skillset r;
    for (uint c = 0; c < r.skills.size(); ++c) {
      r.get((Category)c) = Skill(rand() % 10);
    }
    return r;
  }

  const char* skillname(Category c) {
    switch (c) {
    case BUREAUCRACY: return "BUREAUCRACY";

    case BOT_PROGRAMMING: return "BOT PROGRAMMING";
    case BOT_MAINTENANCE: return "BOT MAINTENANCE";
    case BOT_OPERATION: return "BOT OPERATION";
    case VEHICLE_PROGRAMMING: return "VEHICLE PROGRAMMING";
    case VEHICLE_MAINTENANCE: return "VEHICLE MAINTENANCE";
    case VEHICLE_OPERATION: return "VEHICLE OPERATION";
    case REACTOR_PROGRAMMING: return "REACTOR PROGRAMMING";
    case REACTOR_MAINTENANCE: return "REACTOR MAINTENANCE";
    case REACTOR_DESIGN: return "REACTOR DESIGN";
    case REACTOR_OPERATION: return "REACTOR OPERATION";
    case MINING_LASER: return "MINING LASER";
    case CONSTRUCTION: return "CONSTRUCTION";

    case PHARMACEUTICALS: return "PHARMACEUTICALS";
    case DRUG_ADMINISTRATION: return "DRUG ADMINISTRATION";
    default: return "???";
    }
  }
}
