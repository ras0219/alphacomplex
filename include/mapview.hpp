#pragma once

#include "component.hpp"
#include "city.hpp"
#include <vector>

using std::vector;

struct MapView : Component {
  MapView(int x, int y, City* c)
    : city(c), xsz(x), ysz(y), buf(x*y, '\0'),
      csr_enable(false), csr_x(x/2), csr_y(y/2) { }

  virtual void render(Graphics&);

  void putChar(int x, int y, char c);

  inline void up() { if (csr_y > 1) --csr_y; }
  inline void down() { ++csr_y; if (csr_y >= ysz-1) csr_y = ysz-2; }
  inline void left() { if (csr_x > 1) --csr_x; }
  inline void right() { ++csr_x; if (csr_x >= xsz-1) csr_x = xsz-2; }

  void zap_wall();
  void zap_wall(int, int);

  City* city;
  int xsz, ysz;
  vector<char> buf;

  // For cursors
  bool csr_enable;
  int csr_x, csr_y;
};
