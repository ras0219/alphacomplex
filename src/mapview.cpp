#include "mapview.hpp"
#include "graphics.hpp"
#include "city.hpp"

const char prettywalls[16] = {
  '+', HBAR, VBAR, CORNER_SE,
  HBAR, HBAR, CORNER_SW, TEE_N,
  VBAR, CORNER_NE, VBAR, TEE_W,
  CORNER_NW, TEE_S, TEE_E, CROSS
};

void MapView::render(Graphics& g) {
  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x) {
      assert(city->ent(x,y)->rawname() == SentinelEntity::RAWNAME);
      if (city->ent(x,y)->next != nullptr) {
        putChar(x, y, city->ent(x,y)->next->render());
      } else if (city->tile(x,y).type == '+') {
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
      } else {
        putChar(x, y, city->tile(x,y).type);
      }
    }

  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x)
      g.drawChar(5 + x*6, 15 + y*12, buf[y * xsz + x], Graphics::DEFAULT);
}

void MapView::putChar(int x, int y, char c) {
  if (x >= 0 && x < xsz && y >= 0 && y < ysz)
    buf[y*xsz + x] = c;
}
