#include "views/mapview.hpp"
#include "graphics.hpp"
#include "city.hpp"
#include "clock.hpp"
#include "joblist.hpp"
#include "components/renderable.hpp"
#include "components/furniture.hpp"
#include "components/room.hpp"

#include <cstring>

//#ifdef GRAPHICS_X11
//enum : char {
//  CORNER_SE = 11,
//  CORNER_NE = 12,
//  CORNER_NW = 13,
//  CORNER_SW = 14,
//  CROSS = 15,
//  HBAR = 18,
//  TEE_E = 21,
//  TEE_W = 22,
//  TEE_N = 23,
//  TEE_S = 24,
//  VBAR = 25
//};
//#else
enum : unsigned char {
  Z_FLOOR = 64,
  Z_FLOOR_OVERLAY = 80,
  Z_DEFAULT = 128,
  Z_WALLS = 130,
  Z_FURNITURE = 140,
  Z_ENTITIES = 150,
  Z_TOP = 255
};

enum : char {
  CORNER_SE = '#',
  CORNER_NE = '#',
  CORNER_NW = '#',
  CORNER_SW = '#',
  CROSS = '+',
  HBAR = '-',
  TEE_E = '#',
  TEE_W = '#',
  TEE_N = '#',
  TEE_S = '#',
  VBAR = '|'
};
//#endif

const char prettywalls[16] = {
  '+', HBAR, VBAR, CORNER_SE,
  HBAR, HBAR, CORNER_SW, TEE_N,
  VBAR, CORNER_NE, VBAR, TEE_W,
  CORNER_NW, TEE_S, TEE_E, CROSS
};

void MapView::render(Graphics& g, render_box const& pos) {
  prepare_buffer();
  blit_buffer(g, pos);
}

void MapView::prepare_buffer() {
  // Reset zbuffer
  memset(zbuf.data(), 0, buf.size());

  if (mode == ROOMS) {
    // Draw all the rooms
    for (auto room : city->rooms) {
      rect r = room->r.as_rect();
      if (r.overlaps(vp.as_rect())) {
        for (int y = r.y; y < r.y + r.h; ++y) {
          for (int x = r.x; x < r.x + r.w; ++x) {
            putChar(x, y, '~', Z_FLOOR_OVERLAY);
          }
        }
      }
    }
  }

  for (int y = vp.tly; y < vp.tly + vp.ysz; ++y) {
    for (int x = vp.tlx; x < vp.tlx + vp.xsz; ++x) {
      if (city->designs(x, y) & 1) {
        putChar(x, y, '%', Z_WALLS);
        continue;
      }

      if (mode == ENTCOUNT) {
        if (city->ent(x, y).size() > 0 && city->ent(x, y).size() <= 9) {
          putChar(x, y, (char)('0' + city->ent(x, y).size()), Z_ENTITIES);
          continue;
        }
      } else {
        auto& s = city->ent(x, y);
        auto it = s.begin();
        auto end = s.end();
        for (; it != end; ++it) {
          if ((*it)->has<Renderable>()) {
            putChar(x, y, (*it)->assert_get<Renderable>()->render(), Z_ENTITIES);
            break;
          }
        }

        if (it != end) {
          continue;
        }
      }

      if (city->tile(x, y).type == Tile::wall) {
        int i = 0;
        if (x > 0 && !city->tile(x - 1, y).walkable())
          i += 1;
        if (y > 0 && !city->tile(x, y - 1).walkable())
          i += 2;
        if (x < city->getXSize() - 1 && !city->tile(x + 1, y).walkable())
          i += 4;
        if (y < city->getYSize() - 1 && !city->tile(x, y + 1).walkable())
          i += 8;

        putChar(x, y, prettywalls[i], Z_WALLS);
        continue;
      }

      if (auto f = city->furniture(x, y)) {
        if (f->parent->has<Renderable>()) {
          auto r = f->parent->assert_get<Renderable>();
          putChar(x, y, r->render(), Z_FURNITURE);
          continue;
        }
      }
      putChar(x, y, city->tile(x, y).type, Z_FLOOR);
    }
  }
}

void MapView::blit_buffer(Graphics& g, render_box const& pos) {
  // TODO: actually obey pos's w & h
  for (int y = 0; y < vp.ysz; ++y) {
    for (int x = 0; x < vp.xsz; ++x)
      g.drawChar(pos.x + x, pos.y + y, buf[y * vp.xsz + x]);
  }
}

void MapView::putChar(int x, int y, char c, unsigned char z) {
  x -= vp.tlx;
  y -= vp.tly;
  if (x >= 0 && x < vp.xsz && y >= 0 && y < vp.ysz) {
    if (zbuf[y*vp.xsz + x] < z) {
      zbuf[y*vp.xsz + x] = z;
      buf[y*vp.xsz + x] = c;
    }
  }
}

void MapViewCursor::render(Graphics& g, render_box const& pos) {
  mv->prepare_buffer();
  mv->putChar(csr.x, csr.y, 'X', Z_TOP);
  mv->blit_buffer(g, pos);
}
