#pragma once

#include "entities/entity.hpp"

struct CitizenName : AspectStatic<Aspect::CitizenName, CitizenName> {
  CitizenName(string n, string s)
    : name(n), sector(s) { }

  string name;
  string sector;
};

CitizenName* random_citizenname();
