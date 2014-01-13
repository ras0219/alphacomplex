#include "views/scrollable.hpp"
#include "graphics.hpp"

void Scrollable::render(Graphics& g) {
  uint nrows = num_rows();
  uint rowcap = pos.h;

  //// Layout: ////////////
  // row9999999    ^
  // Blah          #
  // Blah          |
  // Blah4         |
  // Blah9         v
  /////////////////////////

  //// Scrollbar Math /////
  // Top of scrollbar = offset * rowcap / nrows
  // Bottom of scroll = (offset + rowcap) * rowcap / nrows
  //
  // Therefore:
  // offset * rowcap >= R * nrows >= (offset + rowcap) * rowcap
  /////////////////////////

  if (offset + rowcap > nrows)
    offset = nrows - rowcap;
  if (offset < 0)
    offset = 0;

  box rowbox = pos;
  rowbox.w -= 2;
  rowbox.h = 1;

  uint sbtop = offset * rowcap;
  uint sbbot = rowcap * rowcap + sbtop;

  for (uint x = 0; x < rowcap; ++x) {
    if (sbtop <= x * nrows && x * nrows <= sbbot) {
      g.drawChar(pos.x + pos.w - 1, pos.y + x, '#');
    } else {
      if (x == 0) {
        g.drawChar(pos.x + pos.w - 1, pos.y + x, '^');
      } else if (x == rowcap - 1) {
        g.drawChar(pos.x + pos.w - 1, pos.y + x, 'v');
      } else {
        g.drawChar(pos.x + pos.w - 1, pos.y + x, '|');
      }
    }
    if (nrows > x + offset) {
      render_row(g, x + offset, rowbox);
      ++rowbox.y;
    }
  }
}

void Scrollable::page_up() {
  offset -= pos.h;
  if (offset < 0) {
    offset = 0;
  }
}

void Scrollable::page_down() {
  offset += pos.h;
  if (offset + pos.h > (int)num_rows()) {
    offset = num_rows() - pos.h;
  }
}

void Scrollable::move_to_include(uint row) {
  if ((int)row < offset)
    offset = row;
  if (offset + pos.h < (int)row + 1)
    offset = row - pos.h + 1;
}
