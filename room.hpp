#pragma once

struct Room {
  struct City* c;
  int x, y, w, h;

  virtual const char* rawname() = 0;

  Room(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_) { }
  virtual ~Room() { }

  virtual void update() { }
};
