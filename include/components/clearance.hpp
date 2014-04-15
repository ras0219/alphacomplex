#pragma once

#include "entities/entity.hpp"
#include "security.hpp"
#include "department.hpp"

struct clearance {
  Security::Mask security;
  Department::Mask dept;
};

struct Clearance : ecs::ComponentCRTP<ecs::Component::Clearance, Clearance>, clearance {
  Clearance(const clearance& c) : clearance(c) {}

  inline Security::Mask& security() { return clearance::security; }
  inline Department::Mask& department() { return clearance::dept; }
};

inline bool operator&(const clearance& l, const clearance& r) {
  return l.security & r.security && l.dept & r.dept;
}
