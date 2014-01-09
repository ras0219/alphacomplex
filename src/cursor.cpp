#include "cursor.hpp"
#include "city.hpp"

bool CityViewport::check() {
  bool r = true;
  if (tlx >= city->getXSize()) {
    tlx = city->getXSize() - 1;
    r = false;
  }
  if (tly >= city->getYSize()) {
    tly = city->getYSize() - 1;
    r = false;
  }
  return r;
}

bool CityViewport::resize(int nxsz, int nysz) {
  xsz = nxsz;
  ysz = nysz;
  return true;
}
bool CityViewport::move(int ntlx, int ntly) {
  tlx = ntlx;
  tly = ntly;
  return check();
}
bool CityViewport::move_relative(int tlxoff, int tlyoff) {
  tlx += tlxoff;
  tly += tlyoff;
  return check();
}
bool CityViewport::move_to_include(int x, int y) {
  if (x >= tlx + xsz) {
    tlx = x - xsz + 1;
  } else if (x < tlx) {
    tlx = x;
  }

  if (y >= tly + ysz) {
    tly = y - ysz + 1;
  } else if (y < tly) {
    tly = y;
  }

  return check();
}

bool CityCursor::check() {
  int xsz = city->getXSize();
  int ysz = city->getYSize();
  if (x < 1) {
    x = 1;
    return false;
  }
  if (x > xsz - 2) {
    x = xsz - 2;
    return false;
  }
  if (y < 1) {
    y = 1;
    return false;
  }
  if (y > ysz - 2) {
    y = ysz - 2;
    return false;
  }
  return true;
}

bool CityCursor::set(int nx, int ny) {
  x = nx;
  y = ny;
  return check();
}
