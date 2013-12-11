#pragma once

#include "system.hpp"
#include "entity.hpp"

#include <unordered_map>

using std::unordered_map;

template<class Derived, class...Args>
struct SubSystem : System {
  typedef tuple<Args*...> tuple_t;
  typedef unordered_map<Ent*, tuple_t > map_t;
  typedef typename map_t::value_type value_type;

  virtual void insert(Ent* e) override {
    nodes.insert({e, e->get_tuple<Args...>()});
  }
  virtual void erase(Ent* e) override {
    nodes.erase(e);
  }

  virtual void update() override {
    for (auto node : nodes)
      static_cast<Derived*>(this)->update(node);
  }

  map_t nodes;
};
