#pragma once

#include "entities/entity.hpp"

#include <unordered_set>

using std::unordered_set;

struct City;

struct Room : AspectStatic<Aspect::Room, Room> {
  typedef unordered_set<struct Furniture*> set_t;

  Room(City& c, int x_, int y_, int w_, int h_)
    : city(&c), x(x_), y(y_), w(w_), h(h_)
    { }
  ~Room() { deinit(); }

  void init();
  void deinit();

  inline bool contains(int tx, int ty) const {
    return (tx >= x)
      && (tx < x + w)
      && (ty >= y)
      && (ty < y + h);
  }

  City* city;
  int x, y, w, h;

  set_t furniture;
};
