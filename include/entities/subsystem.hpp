#pragma once

#include "system.hpp"
#include "entities/entity.hpp"
#include "utilities/apply_tuple.hpp"
#include "utilities/thunk_func.hpp"
#include <unordered_map>

struct Ent;

template<class Derived, class...Args>
struct SubSystem : System {
  typedef std::unordered_map<Ent*, std::tuple<Args*...>> map_t;
  typedef typename map_t::value_type value_type;

  SubSystem(int tr = 1) : tickcount(0), tickrate(tr) {}

  virtual void insert(Ent* e) override {
      nodes.insert({ e, std::make_tuple(e->get<Args>()...) });
  }

  virtual void erase(Ent* e) override {
      nodes.erase(e);
  }

  virtual void update() override {
      tickcount = (tickcount + 1) % tickrate;
      if (!tickcount) {
          for (auto&& node : nodes) {
              auto tup1 = std::make_tuple(static_cast<Derived*>(this), node.first);
              auto tup = std::tuple_cat(std::move(tup1), node.second);
              auto th = rasutil::thunk(&Derived::update_item);
              rasutil::apply_tuple(std::move(th), std::move(tup));
          }
      }
  }

  int tickcount;
  int tickrate;
  map_t nodes;

  static Derived& singleton() { return g_singleton; }

private:
  static Derived g_singleton;
};

