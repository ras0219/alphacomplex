#pragma once

#include "components/component.hpp"

#include <unordered_set>
#include <unordered_map>

struct System;

struct Ent {
  typedef std::unordered_map<int, Component*> map_t;
  typedef std::unordered_set<System*> set_t;

  ~Ent();
  
  template<class T>
  T* assert_get();

  template<class T>
  T* get();
  template<class T>
  T* has_get();
  
  template<class T>
  bool has();

  template<class...Ts>
  std::tuple<Ts*...> get_tuple();

  bool has(Component::Kind k);
  void add(Component* comp);
  void add(System* sys);

  template<class C, class...Args>
  void emplace(Args&&...args);

  map_t compmap;
  set_t sysset;
};
