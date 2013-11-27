#include "defs.hpp"
#include "city.hpp"
#include "room.hpp"
#include "workroom.hpp"
#include "garbage.hpp"
#include "log.hpp"

#include <stdexcept>

const char* SentinelEntity::RAWNAME = "sentinel";

Room* City::find_room(const char* rawname) {
  for (auto r : rooms) {
    if (r->rawname() == rawname)
      return r;
  }
  return nullptr;
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
    LOGGER::error() << "'" << s << "'" << endl;
    if (s.size() == 0)
      continue;
    if (y == 0)
      x = s.size();

    if (s.size() != (uint)x)
      throw runtime_error("invalid c format");

    for (int i=0; i < x; ++i) {
      c.tiles.push_back( {(Tile::TileKind)s[i]} );
      c.ents.emplace_back(c);
    }

    ++y;
  }

  if (y == 0)
    throw runtime_error("invalid c size");

  c.xsz = x;
  c.ysz = y;

  LOGGER::verbose() << "C is " << x << " by " << y << endl;

  // Now generate rooms
  for (int j=0; j<c.getYSize();++j) {
    for (int i=0; i<c.getXSize();++i) {
      char ch = c.tile(i,j).type;
      if (ch == Tile::wall || ch == Tile::ground) {
      } else if (ch == 'E') {
        Citizen* e = new Citizen(i,j,Security::RED, c);
        e->insert_after(c.ent(i,j));
        c.tile(i,j).type = Tile::ground;
      } else if (ch == 'O') {
        Citizen* e = new Citizen(i,j,Security::ORANGE, c);
        e->insert_after(c.ent(i,j));
        c.tile(i,j).type = Tile::ground;
      } else if (ch == 'D') {
        Dwarf* e = new Dwarf(i,j, c);
        e->insert_after(c.ent(i,j));
        c.tile(i,j).type = Tile::ground;
      } else {
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
        if (ch == 'C')
          c.rooms.push_back(new CleaningRoom(i,j,w,h));
        else
          c.rooms.push_back(new WorkRoom(i,j,w,h,ch));
      }
    }
  }

  for (auto r : c.rooms)
    LOGGER::verbose() << r->w << 'x' << r->h << " Room @ " << r->x << ", " << r->y << endl;

  for (int j=0; j<c.getYSize();++j) {
    for (int i=0; i<c.getXSize();++i)
      LOGGER::verbose() << (char)c.tile(i,j).type;
    LOGGER::verbose() << '\n';
  }

  return is;
}
