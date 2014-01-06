#pragma once

#include "widget.hpp"
#include "city.hpp"
#include <vector>
#include "cursor.hpp"

using std::vector;

struct MapView : Widget {
  enum Mode {
    DEFAULT,
    ENTCOUNT,
    //ROOMS,
    MAX_MODE
  };

  MapView(int x, int y, City& c)
    : city(c), buf(x*y, '\0'), mode(DEFAULT), vp(c, x, y) {}

  // This is just prepare_buffer() followed by blit_buffer(g)
  virtual void render(Graphics&);
  // Fill buf with the text to draw
  void prepare_buffer();
  // Copy buf to graphics
  void blit_buffer(Graphics&);

  void putChar(int x, int y, char c);

  inline void set_mode(Mode m) { mode = m; }
  inline void next_mode() { mode = (Mode)((mode + 1) % MAX_MODE); }

  inline void move_to_include(int x, int y) { vp.move_to_include(x, y); }

  // Shared & Read only
  City& city;
  vector<char> buf;
  Mode mode;
  CityViewport vp;
};

struct MapViewCursor {
  MapViewCursor(MapView& mv) : mv(mv), csr(mv.city) {}

  inline void render(Graphics& g) {
    mv.prepare_buffer();
    mv.putChar(csr.x, csr.y, 'X');
    mv.blit_buffer(g);
  }

  inline void right() { csr.offset(1, 0); update_vp(); }
  inline void left() { csr.offset(-1, 0); update_vp(); }
  inline void up() { csr.offset(0, -1); update_vp(); }
  inline void down() { csr.offset(0, 1); update_vp(); }

  inline void update_vp() { mv.move_to_include(csr.x, csr.y); }

  MapView& mv;

  // For cursors
  bool csr_draw;
  CityCursor csr;
};