#pragma once

#include "components/component.hpp"

#include <unordered_set>

struct Furniture : ComponentCRTP<Component::Furniture, Furniture> {
  using set_t = std::unordered_set<struct Room*>;
  ~Furniture() { deinit(); }

  void init();
  void deinit();

  set_t rooms;
};
