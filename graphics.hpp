#pragma once

#include <unistd.h>
#include <cassert>
#include <X11/Xlib.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

struct Graphics;

struct Component {
  virtual void render(Graphics&) = 0;
};

extern const char* white;

struct Graphics {
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;

  Display *display;
  Window window;
  XEvent event;
  int s;

  Component* c;


  GC white_gc;
  XColor white_col;
  Colormap colormap;

  Graphics() {
    #ifdef NDEBUG
    debug = 0;
    #else
    debug = 1;
    #endif

    /* open connection with the server */
    display = XOpenDisplay(NULL);
    assert(display != nullptr);
 
    s = DefaultScreen(display);

    /* create window */
    window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 200, 200, 1,
                                 BlackPixel(display, s), WhitePixel(display, s));
 
    colormap = DefaultColormap(display, s);
    white_gc = XCreateGC(display, window, 0, 0);
    XParseColor(display, colormap, white, &white_col);
    XAllocColor(display, colormap, &white_col);
    XSetForeground(display, white_gc, white_col.pixel);

    /* select kind of events we are interested in */
    XSelectInput(display, window, ExposureMask | KeyPressMask);
 
    /* map (show) the window */
    XMapWindow(display, window);
  }

  void handle_events() {
    int events = XPending(display);
    while (events > 0) {
      cerr << events << " Events." << endl;
      XNextEvent(display, &event);
 
      /* draw or redraw the window */
      if (event.type == Expose)
        repaint();

      /* exit on key press */
      if (event.type == KeyPress) {
        destroy();
        return;
      }

      events = XPending(display);
    }
  }

  void repaint() {
    XFillRectangle(display, window, white_gc, 30, 30, 120, 120);

    if (c != nullptr)
      c->render(*this);

    for (int y=0;y<12;++y)
      for (int x=0;x<12;++x)
        XDrawString(display, window, DefaultGC(display, s),
                    30 + x*10, 30 + y*10,
                    &buf[y][x], 1);
  }

  bool destroyed = false;
  void destroy() {
    if (destroyed) return;
    XCloseDisplay(display);
    destroyed = true;
  }

  ~Graphics() { destroy(); }

  int debug;
  char buf[12][12];

  void putChar(int x, int y, char c) {
    if (debug > 0)
      cerr << "putChar(" << x << ", " << y << ", '" << c << "')" << endl;

    if (x >= 0 && x < 12 && y >= 0 && y < 12)
      buf[y][x] = c;
  }

  void print() {
    for (int y=0;y<12;++y) {
      for (int x=0;x<12;++x)
        cout << buf[y][x];
      cout << '\n';
    }
  }
  void clear() {
    for (int y=0;y<12;++y)
      for (int x=0;x<12;++x)
        buf[y][x] = '\0';
  }

  void beginDebug() {
    ++debug;
  }
  void endDebug() {
    --debug;
  }
};
