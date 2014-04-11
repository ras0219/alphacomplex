#pragma once

#include "system.hpp"
#include <unordered_map>

struct Ent;

template<class Derived, class...Args>
struct SubSystem : System {
  typedef std::unordered_map<Ent*, std::tuple<Args*...>> map_t;
  typedef typename map_t::value_type value_type;

  SubSystem(int tr = 1);

  virtual void insert(Ent* e) override;
  
  virtual void erase(Ent* e) override;

  virtual void update() override;

  int tickcount;
  int tickrate;
  map_t nodes;
};
