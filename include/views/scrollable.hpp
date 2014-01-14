#pragma once

#include "defs.hpp"
#include "widget.hpp"

struct Scrollable {
  Scrollable() : offset(0), cached_rowcap(1) {}

  void render(Graphics& g, render_box const& pos);

  virtual uint num_rows() const = 0;
  virtual void render_row(Graphics& g, uint rownum, render_box const& pos) = 0;

  void move_to_include(uint row);

protected:
  int offset;
  uint cached_rowcap;
};
