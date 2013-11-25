#pragma once

#include "component.hpp"

#include <vector>

using std::vector;

struct City;

struct MapView : Component {
  MapView(int x, int y, City* c) : city(c), xsz(x), ysz(y), buf(x*y, '\0') { }

  virtual void render(Graphics&);

  void putChar(int x, int y, char c);

  City* city;
  int xsz, ysz;
  vector<char> buf;
};
