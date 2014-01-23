#pragma once

#include "defs.hpp"

struct Controller {
  virtual void handle_keypress(KeyboardKey ks) = 0;
};
