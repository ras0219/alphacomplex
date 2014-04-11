#include "entities/subsystem.hpp"

#include "entities/entity.hpp"
#include "utilities/apply_tuple.hpp"
#include "utilities/thunk_func.hpp"

using std::unordered_map;
using namespace rasutil;

template <class Derived, class...Args>
SubSystem<Derived,Args...>::SubSystem(int tr) : tickcount(0), tickrate(tr) {}

template <class Derived, class...Args>
void SubSystem<Derived,Args...>::insert(Ent* e) {
  nodes.insert({ e, std::make_tuple(e->get<Args>()...) });
}

template <class Derived, class...Args>
void SubSystem<Derived,Args...>::erase(Ent* e) {
  nodes.erase(e);
}

template <class Derived, class...Args>
void SubSystem<Derived,Args...>::update() {
  tickcount = (tickcount + 1) % tickrate;
  if(!tickcount) {
    for(auto&& node : nodes) {
      auto tup1 = std::make_tuple(static_cast<Derived*>(this), node.first);
      auto tup = std::tuple_cat(std::move(tup1), node.second);
      auto th = thunk(&Derived::update_item);
      apply_tuple(std::move(th), std::move(tup));
    }
  }
}
