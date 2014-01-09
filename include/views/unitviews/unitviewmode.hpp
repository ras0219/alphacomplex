#pragma once

#include "defs.hpp"
#include "components/citizenname.hpp"
#include "entities/citizen.hpp"

struct Graphics;

struct UnitViewMode {
  virtual void render(Graphics& g, uint row, uint col) = 0;
  virtual uint num_cols() = 0;
  virtual void toggle(CitizenName::iterator it, uint col_num) = 0;
};

template<class P>
void render_policy(Graphics& g, uint row, uint col) {
  uint row_off = 12;
  uint col_off = 48;
  uint left = 124;
  uint top_row = 42;

  g.drawString(6, 18, P::title());

  uint c = 0;
  for (auto d : P::col_list()) {
    g.drawString(left + 6 + c*col_off, top_row, P::col_label(d));
    ++c;
  }

  uint r = 0;

  for (auto e : CitizenName::instances) {
    g.drawString(18, top_row + 12 + r*row_off, get_full_name(e->parent));

    c = 0;
    for (auto d : P::col_list()) {
      string buf2 = "|      ";
      if (c == col && r == row) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      P::entry(e, d, buf2);

      g.drawString(left + c*col_off, top_row + 12 + r*row_off, buf2);
      ++c;
    }

    ++r;
  }

  // Render cursor
  g.drawChar(6, top_row + 12 + row*row_off, '>', Graphics::DEFAULT);
  g.drawString(left + 18 + col*col_off, 18, "VV", Graphics::DEFAULT);
}

