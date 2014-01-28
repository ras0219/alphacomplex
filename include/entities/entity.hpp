#pragma once

#include "system.hpp"
#include "components/component.hpp"

#include <list>
#include <map>
#include <tuple>
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

struct Ent {
  typedef std::unordered_map<int, Component*> map_t;
  typedef std::unordered_set<System*> set_t;

  ~Ent() {
    for (auto p : compmap)
      p.second->on_remove();

    for (auto p : compmap)
      delete p.second;

    for (auto sys : sysset)
      sys->erase(this);
  }

  template<class T>
  inline T* assert_get() {
    assert(has<T>());
    return get<T>();
  }

  template<class T>
  inline T* get() { return static_cast<T*>(compmap[T::StaticKind]); }
  template<class T>
  inline T* has_get() {
    auto it = compmap.find(T::StaticKind);
    if (it == compmap.end())
      return nullptr;
    return static_cast<T*>(it->second);
  }
  template<class T>
  inline bool has() { return compmap.find(T::StaticKind) != compmap.end(); }

  template<class...Ts>
  inline std::tuple<Ts*...> get_tuple() {
    return std::make_tuple(std::get<Ts>()...);
  }

  inline bool has(Component::Kind k) { return compmap.find(k) != compmap.end(); }
  inline void add(Component* comp) {
    comp->parent = this;
    compmap[comp->kind] = comp;
    comp->on_add();
  }
  inline void add(System* sys) {
    sysset.insert(sys);
    sys->insert(this);
  }

  template<class C, class...Args>
  inline void emplace(Args&&...args) {
    add(new C(std::forward<Args>(args)...));
  }

  map_t compmap;
  set_t sysset;
};
