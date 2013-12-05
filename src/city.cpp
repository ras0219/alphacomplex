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

  c.resize(x, y);

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

struct WalkToDigJob : WalkToJob<WalkToDigJob> {
  WalkToDigJob(int x_, int y_)
    : WalkToJob(x_, y_) { }
  
  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "Digging");
  }
  virtual Department::Mask department() { return Department::FACILITIES; }
  virtual Security::Mask security() { return Security::ALL; }

  virtual bool complete_walk(Citizen* e) {
    return true;
  }

  static const char* RAWNAME;
};
const char* WalkToDigJob::RAWNAME = "walktodigjob";

struct DigJob : ActivityJob<DigJob> {
  DigJob(City* c, int x_, int y_) : city(c), x(x_), y(y_) { }
  
  virtual int description(char* buf, size_t n) const {
    return snprintf(buf, n, "Digging");
  }
  virtual Department::Mask department() { return Department::FACILITIES; }
  virtual Security::Mask security() { return Security::ALL; }
  virtual int duration() { return 20; }
  virtual bool complete_activity(Citizen* e) {
    if (city->designs(x,y) & 1 && city->tile(x,y).type == Tile::wall) {
      city->remove_wall(x,y);
    }
    return true;
  }

  static const char* RAWNAME;
  City* city;
  int x, y;
};
const char* DigJob::RAWNAME = "digjob";

void add_wall_dig_job(City* c, int x1, int y1, int x2, int y2) {
  Job* j = new MultiJob{ new WalkToDigJob(x1, y1),
                         new DigJob(c, x2, y2) };
  jobs.add_job(j);
}

void City::toggle_dig_wall(int x, int y) {
  if (tile(x,y).type == Tile::wall) {
    designs(x,y) ^= 1; // Mark for digging
    if (designs(x,y) & 1) {
      for (auto o : offs) {
        if (tile(x+o.first,y+o.second).walkable()) {
          add_wall_dig_job(this, x+o.first, y+o.second, x, y);
          break;
        }
      }
    }
  }
}
void City::remove_wall(int x, int y) {
  if (tile(x,y).type != Tile::wall)
    return;
  designs(x,y) &= ~1;
  tile(x,y).type = Tile::ground;

  for (auto o : offs) {
    if (!(designs(x+o.first, y+o.second) & 1))
      continue;
    int facings = 0;
    for (auto o2 : offs)
      if (tile(x+o.first+o2.first, y+o.second+o2.second).walkable())
        ++facings;
    if (facings == 1)
      add_wall_dig_job(this, x, y, x+o.first, y+o.second);
  }
}
