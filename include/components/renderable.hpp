#pragma once

#include "components/component.hpp"

struct Renderable :  ComponentCRTP<Component::Renderable, Renderable> {
  Renderable(char c = '?') : ch(c) { }

  inline char render() const { return ch; }
  inline void set_char(char c) { ch = c; }

  char ch;
};
