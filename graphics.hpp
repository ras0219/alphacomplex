#pragma once

#include <iostream>
using namespace std;

struct Graphics {
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
