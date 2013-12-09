#pragma once

#include "entity.hpp"
#include <string>

using std::string;

struct Descriptable : ECompStatic<EComp::Descriptable> {
  inline const string& name() const { return n; }
  inline const string& name() const { return n; }

  string n;
  string sect;
};
