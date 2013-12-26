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
  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x) {
      if (csr_enable && x == csr_x && y == csr_y/* && animtime % 30 < 15*/) {
        putChar(x, y, 'X');
        continue;
      }
      if (city->designs(x,y) & 1) {
        putChar(x, y, '%');
        continue;
      }

      if (mode == DEFAULT) {
        set<Ent*>& s = city->ent(x, y);
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
      }

      if (city->tile(x,y).type == Tile::wall) {
        int i = 0;
        if (x > 0 && !city->tile(x-1,y).walkable())
          i += 1;
        if (y > 0 && !city->tile(x,y-1).walkable())
          i += 2;
        if (x < city->getXSize()-1 && !city->tile(x+1,y).walkable())
          i += 4;
        if (y < city->getYSize()-1 && !city->tile(x,y+1).walkable())
          i += 8;
        putChar(x, y, prettywalls[i]);
        continue;
      }

      if (mode == ENTCOUNT) {
        putChar(x, y, '0' + city->ent(x, y).size());
        continue;
      }

      putChar(x, y, city->tile(x,y).type);
    }

  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x)
      g.drawChar(5 + x*6, 15 + y*12, buf[y * xsz + x], Graphics::DEFAULT);
}

void MapView::putChar(int x, int y, char c) {
  if (x >= 0 && x < xsz && y >= 0 && y < ysz)
    buf[y*xsz + x] = c;
}

void MapView::zap_wall() { zap_wall(csr_x, csr_y); }
void MapView::zap_wall(int x, int y) {
  city->toggle_dig_wall(x,y);
}
