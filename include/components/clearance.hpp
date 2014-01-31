#pragma once

#include "entities/entity.hpp"
#include "security.hpp"
#include "department.hpp"

struct clearance {
  Security::Mask security;
  Department::Mask dept;
};

struct Clearance : ComponentCRTP<Component::Clearance, Clearance> {
  Clearance(struct clearance c) : clear(c) { }

  inline struct clearance& get_clearance() { return clear; }
  inline Security::Mask& security() { return clear.security; }
  inline Department::Mask& department() { return clear.dept; }

  struct clearance clear;
};

inline bool operator&(const clearance& l, const clearance& r) {
  return l.security & r.security && l.dept & r.dept;
}
