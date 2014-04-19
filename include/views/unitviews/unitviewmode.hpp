#pragma once

#include "defs.hpp"
#include "components/citizenname.hpp"
#include "entities/citizen.hpp"
#include "views/scrollable.hpp"

#include "graphics.hpp"

struct UnitViewMode : Scrollable {
  using Scrollable::render;

  virtual void render(Graphics& g, render_box const& pos, uint csr_row, uint csr_col) = 0;

  inline void page_up(uint& csr_row) {
    if (csr_row < cached_rowcap) {
      csr_row = 0;
    } else {
      csr_row -= cached_rowcap;
    }
  }

  inline void page_down(uint& csr_row) {
    if (csr_row + cached_rowcap > num_rows()) {
      csr_row = num_rows() - 1;
    } else {
      csr_row += cached_rowcap;
    }
  }

  virtual uint num_cols() const = 0;
  virtual void toggle(CitizenName::iterator it, uint col_num) = 0;
};

template<class P>
struct UnitViewModeInstance : UnitViewMode {
  uint csr_row_cache;
  uint csr_col_cache;

  virtual uint num_rows() const override { return CitizenName::instances.size(); }

  virtual void render_row(Graphics& g, uint rownum, const render_box& pos) override {
    uint col_off = 8;

    auto&& e = CitizenName::instances[rownum];
    g.drawString(pos.x + 2, pos.y, get_full_name(e->parent));

    uint c = 0;
    for (auto&& d : P::col_list()) {
      string buf2 = "|      ";
      if (c == csr_col_cache && rownum == csr_row_cache) {
        buf2[1] = '>';
        buf2[6] = '<';
      }
      P::entry(e, d, buf2);

      g.drawString(pos.x + 19 + c*col_off, pos.y, buf2);
      ++c;
    }

    if (csr_row_cache == rownum) {
      // This row should have the cursor on it
      g.drawChar(pos.x, pos.y, '>');
    }
  }

  virtual void render(Graphics& g, render_box const& pos, uint csr_row, uint csr_col) override {
    Scrollable::move_to_include(csr_row);

    csr_row_cache = csr_row;
    csr_col_cache = csr_col;

    uint col_off = 8;
    const uint left_col_width = 15;
    const uint left = left_col_width;

    g.drawString(pos.x, pos.y, P::title());

    uint c = 0;
    for (auto d : P::col_list()) {
      g.drawString(pos.x + left + c*col_off + 5, pos.y + 1, P::col_label(d));
      ++c;
    }

    render_box pos2 = pos;
    pos2.shrink_top(2);
    Scrollable::render(g, pos2);

    // Render cursor
    g.drawString(pos.x + left + 2 + 5 + csr_col*col_off, pos.y, "VV", false, Graphics::colors_to_context[Graphics::YELLOW]);

  }
};
