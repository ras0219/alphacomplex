#pragma once

#include "entities/entity.hpp"

struct Room;

struct Furniture : AspectStatic<Aspect::Furniture, Furniture> {
  typedef unordered_set<struct Room*> set_t;
  ~Furniture() { deinit(); }

  void init();
  void deinit();

  set_t rooms;
};
