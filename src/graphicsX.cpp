#include "graphics.hpp"
#include "component.hpp"
#include "controller.hpp"
#include "log.hpp"

#include <unistd.h> //let mingw handle it if needed
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>

#include <X11/keysym.h>

using namespace std;
using namespace chrono;

struct GraphicsInternal {
  Window window;

  GC white_gc;
  XColor white_col;
  Colormap colormap;

  int width, height;
};

Graphics::Graphics() : pImpl(new GraphicsInternal()) {
  /* open connection with the server */
  display = XOpenDisplay(NULL);
  assert(display != nullptr);
 
  s = DefaultScreen(display);

  /* create window */
  pImpl->width = 800;
  pImpl->height = 600;

  pImpl->window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10,
                                      pImpl->width, pImpl->height, 1,
                                      BlackPixel(display, s), WhitePixel(display, s));

  pImpl->colormap = DefaultColormap(display, s);
  pImpl->white_gc = XCreateGC(display, pImpl->window, 0, 0);
  XParseColor(display, pImpl->colormap, white, &pImpl->white_col);
  XAllocColor(display, pImpl->colormap, &pImpl->white_col);
  XSetForeground(display, pImpl->white_gc, pImpl->white_col.pixel);

  /* select kind of events we are interested in */
  XSelectInput(display, pImpl->window,
               ExposureMask
               | KeyPressMask
               | StructureNotifyMask);
 
  /* map (show) the window */
  XMapWindow(display, pImpl->window);
}
void Graphics::LoadText(const std::string msg, const std::string font_file)
{
  return;
}
void Graphics::handle_events(Controller* c) {
  XEvent event;

  int events = XPending(display);

  while (events > 0) {
    LOGGER::debug() << events << " Events." << endl;
    XNextEvent(display, &event);
 
    switch (event.type) {
    case Expose:
//      if (event.xexpose.count == 0)
//        repaint();
      break;
    case KeyPress: {
      auto keycode = event.xkey.keycode;
      auto keysym = XKeycodeToKeysym(display, keycode, 0);
      c->handle_keypress(keysym);
      if (destroyed) return;
      break;
    }
    case ConfigureNotify:
      pImpl->width = event.xconfigure.width;
      pImpl->height = event.xconfigure.height;
      break;
    default:
      cerr << "Unhandled event: " << event.type << endl;
      break;
    }

    events = XPending(display);
  }
}
void Graphics::drawString(int x, int y, const string & str, const Graphics::Context gc) {
  GC* t;
  switch (gc) {
  case WHITE:
    t = &pImpl->white_gc;
    break;
  default:
    t = &DefaultGC(display, s);
    break;
  }
  XDrawString(display,
              pImpl->window,
              *t,
              x, y,
              str.c_str(),
              str.length());
}

void Graphics::drawChar(int x, int y, char ch, const Graphics::Context gc) {
  GC* t;
  switch (gc) {
  case WHITE:
    t = &pImpl->white_gc;
    break;
  default:
    t = &DefaultGC(display, s);
    break;
  }
  XDrawString(display,
              pImpl->window,
              *t,
              x, y,
              &ch,
              1);
}

void Graphics::repaint() {
  XFillRectangle(display, pImpl->window, pImpl->white_gc, 0, 0, getWidth(), getHeight());
  
  for (auto p : c)
    p->render(*this);
}

void Graphics::destroy() {
  if (destroyed) return;
  XCloseDisplay(display);
  destroyed = true;
}

Graphics::~Graphics() { 
  destroy();
  delete pImpl;
  pImpl = 0;
}

int Graphics::getWidth() { return pImpl->width; }
int Graphics::getHeight() { return pImpl->height; }
