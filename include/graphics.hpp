#pragma once
#include <deque>
#include <vector>
#include <string>
#include "debug_policy.hpp"
using namespace std;

extern const char* white;

struct Component;
struct _XDisplay;
typedef struct _XDisplay Display;
struct GraphicsInternal;

struct Graphics : Debug<false> {
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics(int x = 12, int y = 12);
  ~Graphics();

  enum Context {
    WHITE,
    DEFAULT
  };
  void drawString(int x, int y, const std::string & str, Context gc = DEFAULT);

  Display *display;
  int s;

  deque<Component*> c;

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
  
  GraphicsInternal* pImpl;
};
