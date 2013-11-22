#include "defs.hpp"
#include "city.hpp"

#include <stdexcept>

const char* SentinelEntity::RAWNAME = "sentinel";

City city;

istream& operator>>(istream& is, City& c) {
  c.tiles.clear();
  c.ents.clear();
  c.xsz = 0;
  c.ysz = 0;

  int x = 0;
  int y = 0;

  string s;
  while (getline(is, s)) {
    cerr << "'" << s << "'" << endl;
    if (s.size() == 0)
      continue;
    if (y == 0)
      x = s.size();
    ++y;

    if (s.size() != (uint)x)
      throw runtime_error("invalid c format");

    for (int i=0; i < x; ++i) {
      c.tiles.push_back({(Tile::TileKind)s[i]});
      c.ents.emplace_back();
    }
  }

  if (y == 0)
    throw runtime_error("invalid c size");

  c.xsz = x;
  c.ysz = y;

  cout << "C is " << x << " by " << y << endl;

  for (int j=0; j<c.getYSize();++j) {
    for (int i=0; i<c.getXSize();++i)
      cout << (char)c.tile(i,j).type;
    cout << '\n';
  }

  return is;
}
