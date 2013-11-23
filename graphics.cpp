#include "graphics.hpp"

Graphics::Graphics(int x, int y) : xsz(x), ysz(y), buf(x*y) {
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
  window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 400, 200, 1,
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

void Graphics::handle_events() {
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

void Graphics::repaint() {
  XFillRectangle(display, window, white_gc, 0, 0, 400, 200);
  
  for (auto p : c)
    p->render(*this);

  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x)
      XDrawString(display, window, DefaultGC(display, s),
		  5 + x*10, 15 + y*10,
		  &buf[y * xsz + x], 1);
}

void Graphics::destroy() {
    if (destroyed) return;
    XCloseDisplay(display);
    destroyed = true;
  }

Graphics::~Graphics() { destroy(); }                                                                                                                     
void Graphics::putChar(int x, int y, char c) {
  if (debug > 0)
    cerr << "putChar(" << x << ", " << y << ", '" << c << "')" << endl;

  if (x >= 0 && x < xsz && y >= 0 && y < ysz)
    buf[y*xsz + x] = c;
}

void Graphics::print() {
  for (int y=0;y<ysz;++y) {
    for (int x=0;x<xsz;++x)
      cout << buf[y*xsz + x];
    cout << '\n';
  }
}

void Graphics::clear() {
  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x)
      buf[y*xsz + x] = '\0';
}

void Graphics::beginDebug() {
  ++debug;
}

void Graphics::endDebug() {
  --debug;
}
