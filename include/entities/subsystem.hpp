#pragma once

#include "system.hpp"
#include "entity.hpp"
#include "utilities/apply_tuple.hpp"
#include "utilities/thunk_func.hpp"
#include <unordered_map>

using std::unordered_map;
using namespace rasutil;

template<class Derived, class...Args>
struct SubSystem : System {
  typedef unordered_map<Ent*, std::tuple<Args*...>> map_t;
  typedef typename map_t::value_type value_type;

  SubSystem(int tr = 1) : tickcount(0), tickrate(tr) { }

  virtual void insert(Ent* e) override {
    nodes.insert({ e, make_tuple(e->get<Args>()...) });
  }
  virtual void erase(Ent* e) override {
    nodes.erase(e);
  }

  virtual void update() override {
    tickcount = (tickcount + 1) % tickrate;
    if (tickcount == 0)
    for (auto node : nodes)
      apply_tuple(thunk(&Derived::update_item), std::tuple_cat(make_tuple(*static_cast<Derived*>(this), node.first), node.second));
  }

  int tickcount;
  int tickrate;
  map_t nodes;
};
