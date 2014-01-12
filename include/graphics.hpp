#pragma once
#include <deque>
#include <vector>
#include <string>
#include "defs.hpp"

using namespace std;

extern const char* white;

#ifdef GRAPHICS_X11
enum : char {
  CORNER_SE = 11,
  CORNER_NE = 12,
  CORNER_NW = 13,
  CORNER_SW = 14,
  CROSS = 15,
  HBAR = 18,
  TEE_E = 21,
  TEE_W = 22,
  TEE_N = 23,
  TEE_S = 24,
  VBAR = 25
};
#else
enum : char {
  CORNER_SE = '#',
  CORNER_NE = '#',
  CORNER_NW = '#',
  CORNER_SW = '#',
  CROSS = '+',
  HBAR = '-',
  TEE_E = '#',
  TEE_W = '#',
  TEE_N = '#',
  TEE_S = '#',
  VBAR = '|'
};
#endif

struct Widget;

struct GraphicsImpl;

struct Graphics : debug_policy_t {
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;

  enum Context {
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    BROWN,
    ORANGE,
    DEFAULT
  };
  void drawString(int x, int y, const std::string& str, Context gc = DEFAULT);
  void drawChar(int x, int y, char str, Context gc = DEFAULT);

  void handle_events(struct Controller*);

  void LoadText(const std::string msg, const std::string font_file);
  void repaint();
  void destroy();

  inline int getWidth() { return width; }
  inline int getHeight() { return height; }

  int width, height;

  deque<Widget*> c;

  bool destroyed = false;

protected:

  Graphics() { }
  ~Graphics() { destroy(); }
};

Graphics* new_graphics();
