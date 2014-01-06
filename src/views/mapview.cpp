#include "views/mapview.hpp"
#include "graphics.hpp"
#include "city.hpp"
#include "clock.hpp"
#include "joblist.hpp"
#include "components/renderable.hpp"

const char prettywalls[16] = {
  '+', HBAR, VBAR, CORNER_SE,
  HBAR, HBAR, CORNER_SW, TEE_N,
  VBAR, CORNER_NE, VBAR, TEE_W,
  CORNER_NW, TEE_S, TEE_E, CROSS
};

void MapView::render(Graphics& g) {
  prepare_buffer(); blit_buffer(g);
}
void MapView::prepare_buffer() {
  for (int y = vp.tly; y < vp.ysz; ++y) {
    for (int x = vp.tlx; x < vp.xsz; ++x) {
      if (city.designs(x, y) & 1) {
        putChar(x, y, '%');
        continue;
      }

      if (mode == DEFAULT) {
        set<Ent*>& s = city.ent(x, y);
        auto it = s.begin();
        while (it != s.end()) {
          if ((*it)->has<Renderable>()) {
            putChar(x, y, (*it)->get<Renderable>()->render());
            break;
          }
          ++it;
        }

        if (it != s.end())
          continue;
      } else if (mode == ENTCOUNT) {
        if (city.ent(x, y).size() > 0) {
          putChar(x, y, '0' + city.ent(x, y).size());
          continue;
        }
      }

      if (city.tile(x, y).type == Tile::wall) {
        int i = 0;
        if (x > 0 && !city.tile(x - 1, y).walkable())
          i += 1;
        if (y > 0 && !city.tile(x, y - 1).walkable())
          i += 2;
        if (x < city.getXSize() - 1 && !city.tile(x + 1, y).walkable())
          i += 4;
        if (y < city.getYSize() - 1 && !city.tile(x, y + 1).walkable())
          i += 8;

        putChar(x, y, prettywalls[i]);
        continue;
      }

      putChar(x, y, city.tile(x, y).type);
    }
  }
}

void MapView::blit_buffer(Graphics& g) {
  for (int y = 0; y < vp.ysz; ++y) {
    for (int x = 0; x < vp.xsz; ++x)
      g.drawChar(5 + x * 6, 15 + y * 12, buf[y * vp.xsz + x], Graphics::DEFAULT);
  }
}

void MapView::putChar(int x, int y, char c) {
  x -= vp.tlx;
  y -= vp.tly;
  if (x >= 0 && x < vp.xsz && y >= 0 && y < vp.ysz)
    buf[y*vp.xsz + x] = c;
}
