#pragma once

#include <unordered_set>

template<class Derived>
struct global_set {
  using set_t = std::unordered_set<Derived*>;
  using iterator = typename set_t::iterator;

  global_set() { globset.insert(static_cast<Derived*>(this)); }
  ~global_set() { globset.erase(static_cast<Derived*>(this)); }

  // global statics

  static iterator begin() { return globset.begin(); }
  static iterator end() { return globset.end(); }

  static set_t globset;
};

template<class Derived>
typename global_set<Derived>::set_t global_set<Derived>::globset;