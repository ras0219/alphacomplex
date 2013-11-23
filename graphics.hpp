#pragma once

#include <unistd.h>
#include <cassert>
#include <X11/Xlib.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>

#include "component.hpp"

using namespace std;
using namespace chrono;

extern const char* white;

struct Graphics {
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics(int x = 12, int y = 12);
  ~Graphics();

  Display *display;
  Window window;
  XEvent event;
  int s;

  deque<Component*> c;

  GC white_gc;
  XColor white_col;
  Colormap colormap;

  void handle_events();
  bool destroyed = false;

  void repaint();
  void destroy();


  int debug;
  int xsz, ysz;
  vector<char> buf;

  void putChar(int x, int y, char c);
  void print();
  void clear();

  void beginDebug();
  void endDebug();
};
