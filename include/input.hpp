#pragma once

#include <X11/Xlib.h>

struct Controller {
  virtual void handle_keypress(KeySym ks) = 0;
};
