#include "graphics.hpp"
#include "component.hpp"
#include "input.hpp"

#include <unistd.h> //let mingw handle it if needed
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <deque>

#include <X11/keysym.h>
#include "SDL.h"

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

}

void Graphics::handle_events(Controller* c) {

}
void Graphics::drawString(int x, int y, const string & str, const Graphics::Context gc) {
}

void Graphics::drawChar(int x, int y, char ch, const Graphics::Context gc) {
}

void Graphics::repaint() {
  
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
