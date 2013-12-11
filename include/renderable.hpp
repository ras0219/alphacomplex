#pragma once

#include "entity.hpp"

struct Renderable :  AspectStatic<Aspect::Renderable, Renderable> {
  Renderable(char c = '?') : ch(c) { }

  inline char render() const { return ch; }
  inline void set_char(char c) { ch = c; }

  char ch;
};
