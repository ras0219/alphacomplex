#pragma once

#include "entity.hpp"
#include "security.hpp"
#include "department.hpp"

struct Clearance {
  Security::Mask security;
  Department::Mask dept;
};

struct ClearanceComp : AspectStatic<Aspect::Clearance, ClearanceComp> {
  ClearanceComp(struct Clearance c) : clear(c) { }

  inline struct Clearance& clearance() { return clear; }
  inline Security::Mask& security() { return clear.security; }
  inline Department::Mask& department() { return clear.dept; }

  struct Clearance clear;
};

inline bool operator&(const Clearance& l, const Clearance& r) {
  return l.security & r.security && l.dept & r.dept;
}
