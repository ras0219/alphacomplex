#include "defs.hpp"
#include "city.hpp"
#include "room.hpp"
#include "workroom.hpp"

#include <stdexcept>

const char* SentinelEntity::RAWNAME = "sentinel";

City city;

void City::render(Graphics& g) {
  assert(this == &city);
  for (int y=0;y<ysz;++y)
    for (int x=0;x<xsz;++x) {
      assert(ent(x,y)->rawname() == SentinelEntity::RAWNAME);
      if (ent(x,y)->next != nullptr) {
        ent(x,y)->next->render(g);
      } else {
        g.putChar(x, y, tile(x,y).type);
      }
    }
}

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
      switch (s[i]) {
      case '+':
        c.tiles.push_back( {Tile::wall} );
        break;
      case '.':
        c.tiles.push_back( {Tile::ground} );
        break;
      default:
        c.tiles.push_back( {(Tile::TileKind)s[i]} );
        break;
      }
      c.ents.emplace_back();
    }
  }

  if (y == 0)
    throw runtime_error("invalid c size");

  c.xsz = x;
  c.ysz = y;

  cout << "C is " << x << " by " << y << endl;

  // Now generate rooms
  for (int j=0; j<c.getYSize();++j) {
    for (int i=0; i<c.getXSize();++i) {
      char ch = c.tile(i,j).type;
      if (ch != Tile::wall && ch != Tile::ground) {
        if (i > 0 && c.tile(i-1,j).type == ch)
          continue;
        if (j > 0 && c.tile(i,j-1).type == ch)
          continue;
        // figure out how wide and high the room is
        int k;
        for (k = i+1; k < c.getXSize(); ++k) {
          if (c.tile(k,j).type != ch)
            break;
        }
        int w = k-i;
        for (k = j+1; k < c.getYSize(); ++k) {
          if (c.tile(i,k).type != ch)
            break;
        }
        int h = k-j;

        // room's top-left is i,j and dimensions are w x h
        c.rooms.push_back(new WorkRoom(i,j,w,h,ch));
      }
    }
  }

  for (auto r : c.rooms)
    cout << r->w << 'x' << r->h << " Room @ " << r->x << ", " << r->y << endl;

  for (int j=0; j<c.getYSize();++j) {
    for (int i=0; i<c.getXSize();++i)
      cout << (char)c.tile(i,j).type;
    cout << '\n';
  }

  return is;
}
