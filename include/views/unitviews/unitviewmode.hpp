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
  uint col_off = 7;
  uint left = 12;
  uint top_row = 2;

  g.drawString(0, 0, P::title());

  uint c = 0;
  for (auto d : P::col_list()) {
    g.drawString(left + (c + 1)*col_off + 1, top_row, P::col_label(d));
    ++c;
  }

  uint r = 0;

  for (auto e : CitizenName::instances) {
    g.drawString(2, top_row + 1 + r, get_full_name(e->parent));

    c = 0;
    for (auto d : P::col_list()) {
      string buf2 = "|      ";
      if (c == col && r == row) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      P::entry(e, d, buf2);

      g.drawString(left + (c + 1)*col_off, top_row + 1 + r, buf2);
      ++c;
    }

    ++r;
  }

  // Render cursor
  g.drawChar(0, top_row + 1 + row, '>', Graphics::DEFAULT);
  g.drawString(left + 3 + (col + 1)*col_off, 0, "VV", Graphics::DEFAULT);
}

