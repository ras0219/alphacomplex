#pragma once

#include "system.hpp"
#include "entity.hpp"

#include <unordered_map>

using std::unordered_map;

template<class Derived, class Arg>
struct SubSystem : System {
  typedef unordered_map<Ent*, Arg*> map_t;
  typedef typename map_t::value_type value_type;

  SubSystem(int tr = 1) : tickcount(0), tickrate(tr) { }

  virtual void insert(Ent* e) override {
    nodes.insert({e, e->get<Arg>()});
  }
  virtual void erase(Ent* e) override {
    nodes.erase(e);
  }

  virtual void update() override {
    tickcount = (tickcount + 1) % tickrate;
    if (tickcount == 0)
      for (auto node : nodes)
        static_cast<Derived*>(this)->update(node);
  }

  int tickcount;
  int tickrate;
  map_t nodes;
};
