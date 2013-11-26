#pragma once

#include "defs.hpp"

struct Controller {
  virtual void handle_keypress(KeySym ks) = 0;
};
