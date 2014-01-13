#pragma once

#include "defs.hpp"
#include "components/citizenname.hpp"
#include "entities/citizen.hpp"
#include "views/scrollable.hpp"

struct Graphics;

struct UnitViewMode {
  virtual void render(Graphics& g, uint csr_row, uint csr_col) = 0;

  virtual uint num_cols() const = 0;
  virtual void toggle(CitizenName::iterator it, uint col_num) = 0;
};

template<class P>
struct UnitViewModeInstance : UnitViewMode, Scrollable {
  uint csr_row_cache;
  uint csr_col_cache;

  virtual uint num_rows() const override { return CitizenName::instances.size(); }

  virtual void render_row(Graphics& g, uint rownum, const box& pos) {
    uint col_off = 8;

    auto e = CitizenName::instances[rownum];
    g.drawString(pos.x, pos.y, get_full_name(e->parent));

    uint c = 0;
    for (auto d : P::col_list()) {
      string buf2 = "|      ";
      if (c == csr_col_cache && rownum == csr_row_cache) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      P::entry(e, d, buf2);

      g.drawString(pos.x + 19 + c*col_off, pos.y, buf2);
      ++c;
    }
  }

  virtual void render(Graphics& g, uint csr_row, uint csr_col) override {
    csr_row_cache = csr_row;
    csr_col_cache = csr_col;

    uint col_off = 8;
    const uint left_reserve = 2;
    const uint left_col_width = 15;
    const uint left = left_reserve + left_col_width;
    uint top_row = 2;

    g.drawString(0, 0, P::title());

    uint c = 0;
    for (auto d : P::col_list()) {
      g.drawString(left + c*col_off + 5, top_row, P::col_label(d));
      ++c;
    }

    pos.x = left_reserve;
    pos.y = top_row + 1;
    pos.h = g.getHeight() - 20;
    pos.w = g.getWidth() - left_reserve;
    Scrollable::render(g);

    // Render cursor
    g.drawChar(0, top_row + 1 + csr_row, '>', Graphics::DEFAULT);
    g.drawString(left + 2 + 5 + csr_col*col_off, 0, "VV", Graphics::DEFAULT);

  }
};
