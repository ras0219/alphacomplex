#pragma once

#include "defs.hpp"
#include "widget.hpp"

struct Scrollable : Widget {
  Scrollable() : offset(0) {}

  virtual void render(Graphics& g) override;

  virtual uint num_rows() const = 0;
  virtual void render_row(Graphics& g, uint rownum, const render_box& pos) = 0;

  void page_up();
  void page_down();
  void move_to_include(uint row);

protected:
  render_box pos;
  int offset;
};
