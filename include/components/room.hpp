#pragma once

#include "components/component.hpp"
#include "point.hpp"
#include <unordered_set>

struct City;

struct Room : ComponentCRTP<Component::Room, Room> {
  typedef std::unordered_set<struct Furniture*> set_t;

  Room(const Rect& r) : r(r) { }
  ~Room() { deinit(); }

  void init();
  void deinit();

  inline bool contains(int tx, int ty) const {
    return (tx >= r.x)
      && (tx < r.x + r.w)
      && (ty >= r.y)
      && (ty < r.y + r.h);
  }

  Rect r;
  set_t furniture;
};
