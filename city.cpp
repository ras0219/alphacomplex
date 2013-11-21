#include "city.hpp"

const char* SentinelEntity::RAWNAME = "sentinel";

City city(12,12);

istream& operator>>(istream& is, City& city) {
  for (int r=0;r<city.getYSize();r++) {
    for (int c=0;c<city.getXSize();c++) {
      char ch;
      is >> ch;
      if (ch == '@') {
        city.tile(c,r).type = (Tile::TileKind)'.';
      } else {
        city.tile(c,r).type = (Tile::TileKind)ch;
      }
    }
  }
  return is;
}
