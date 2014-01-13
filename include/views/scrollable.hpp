#pragma once

#include "defs.hpp"
#include "widget.hpp"

struct Scrollable : Widget {
  Scrollable() : offset(0), cached_rowcap(1) {}

  virtual void render(Graphics& g, render_box const& pos) override;

  virtual uint num_rows() const = 0;
  virtual void render_row(Graphics& g, uint rownum, render_box const& pos) = 0;

  void page_up();
  void page_down();
  void move_to_include(uint row);

protected:
  int offset;

private:
  uint cached_rowcap;
};
