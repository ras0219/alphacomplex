#pragma once

struct City;

struct CityViewport {
  CityViewport(City& city, int xsz = 1, int ysz = 1, int tlx = 0, int tly = 0)
  : city(city), xsz(xsz), ysz(ysz), tlx(tlx), tly(tly) {}

  bool resize(int nxsz, int nysz);
  bool move(int ntlx, int ntly);
  bool move_relative(int tlxoff, int tlyoff);
  bool move_to_include(int x, int y);

  // Externals should not need this
  bool check();

  // Read only
  City& city;
  int xsz, ysz, tlx, tly;
};

struct CityCursor {
  CityCursor(City& city, int x = 1, int y = 1)
  : city(city), x(x), y(y) {}

  inline bool offset(int xoff, int yoff) { x += xoff; y += yoff; return check(); }
  bool set(int nx, int ny);

  // Externals should not need this
  bool check();

  // Read only
  City& city;
  int x, y;
};
