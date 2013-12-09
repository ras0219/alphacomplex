#pragma once

#include "entity.hpp"
#include "security.hpp"
#include "department.hpp"

struct Clearance {
  Security::Mask security;
  Department::Mask dept;
};

struct ClearanceComp : ECompStatic<EComp::Clearance> {
  ClearanceComp(Clearance c) : clear(c) { }

  inline Clearance clearance() const { return clear; }
  inline Security::Mask security() const { return clear.security; }
  inline Department::Mask department() const { return clear.dept; }

  Clearance clear;
};

inline bool operator&(const Clearance& l, const Clearance& r) {
  return l.security & r.security && l.dept & r.dept;
}
