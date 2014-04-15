#pragma once

#include "entities/entity.hpp"

#include <string>

struct CitizenName : ecs::ComponentCRTP<ecs::Component::CitizenName, CitizenName> {
  CitizenName(std::string n, std::string s)
    : name(n), sector(s) { }

  std::string name;
  std::string sector;
};

CitizenName* random_citizenname();
