#include "activityai.hpp"
#include "callbackai.hpp"
#include "citizen.hpp"
#include "city.hpp"
#include "defs.hpp"
#include "furniture.hpp"
#include "garbage.hpp"
#include "ifai.hpp"
#include "ifai.hpp"
#include "log.hpp"
#include "pathai.hpp"
#include "room.hpp"
#include "sequenceai.hpp"
#include "windows.hpp"
#include "workroom.hpp"

#include <stdexcept>

//const char* SentinelEntity::RAWNAME = "sentinel";

Room* City::find_room(const char* rawname) {
  // for (auto r : rooms) {
  //   if (r->rawname() == rawname)
  //     return r;
  // }
  (void) rawname;
  return nullptr;
}

void City::resize(int x, int y) {
  xsz = x;
  ysz = y;
  designs.resize(x, y);
}

wistream& operator>>(wistream& is, City& c) {
  c.tiles.clear();
  c.ents.clear();
  c.xsz = 0;
  c.ysz = 0;

  int x = 0;
  int y = 0;

  wstring s;
  while (getline(is, s)) {
    // LOGGER::error() << "'" << s. << "'" << endl;
    if (s.size() == 0)
      continue;
    if (y == 0)
      x = s.size();

    if (s.size() != (uint)x)
      throw runtime_error("invalid c format");

    for (int i = 0; i < x; ++i) {
      c.tiles.push_back({ (Tile::TileKind)s[i] });
      c.ents.emplace_back();
    }

    ++y;
  }

  if (y == 0)
    throw runtime_error("invalid c size");

  c.resize(x, y);

  LOGGER::verbose() << "C is " << x << " by " << y << endl;

  // Now generate rooms
  for (int j = 0; j < c.getYSize(); ++j) {
    for (int i = 0; i < c.getXSize(); ++i) {
      char ch = c.tile(i, j).type;
      if (ch == Tile::wall || ch == Tile::ground) {
      } else if (ch == 'E') {
        // Citizen* e = new Citizen(i,j,Security::RED, c);
        // e->insert_after(c.ent(i,j));
        c.ent(i, j).insert(new_citizen({ i, j, &c }, Security::RED));
        c.tile(i, j).type = Tile::ground;
      } else if (ch == 'O') {
        // Citizen* e = new Citizen(i,j,Security::ORANGE, c);
        // e->insert_after(c.ent(i,j));
        c.ent(i, j).insert(new_citizen({ i, j, &c }, Security::ORANGE));
        c.tile(i, j).type = Tile::ground;
      } else if (ch == 'D') {
        // Dwarf* e = new Dwarf(i,j, c);
        // e->insert_after(c.ent(i,j));
        c.tile(i, j).type = Tile::ground;
      } else {
        // figure out how wide and high the room is
        int kx, ky, w, h;
        for (kx = i; kx < c.getXSize(); ++kx) {
          if (c.tile(kx, j).type != ch)
            break;
        }
        for (ky = j; ky < c.getYSize(); ++ky) {
          for (int x = i; x < kx; ++x) {
            if (c.tile(x, ky).type != ch)
              goto finish_loop;
          }
        }
        finish_loop:
        w = kx - i;
        h = ky - j;

        for (int y = j; y < ky; ++y) {
          for (int x = i; x < kx; ++x) {
            c.tile(x, y).type = Tile::ground;
          }
        }

        // room's top-left is i,j and dimensions are w x h
        c.rooms.push_back(make_workroom(c, i, j, w, h)->assert_get<Room>());
      }
    }
  }

  for (auto r : c.rooms)
    LOGGER::verbose() << r->w << 'x' << r->h << " Room @ " << r->x << ", " << r->y << endl;

  for (int j = 0; j < c.getYSize(); ++j) {
    for (int i = 0; i < c.getXSize(); ++i)
      LOGGER::verbose() << (char)c.tile(i, j).type;
    LOGGER::verbose() << '\n';
  }

  return is;
}

// struct DigAI : AIState {
//   DigAI(City* c, int dx, int dy, int wx, int wy)
//     : city(c), digx(dx), digy(dy), walkx(wx), walky(wy) { }

//   virtual int start(Citizen* c) {
//     return c->push_aistate(c->path_activity_script(walkx, walky, 100));
//   }
//   virtual int update(Citizen* c) {
//     assert(false); return -1;
//   }
//   virtual int resume(Citizen* c, AIState*) {
//     if (city->designs(digx,digy) & 1 && city->tile(digx,digy).type == Tile::wall) {
//       city->remove_wall(digx,digy);
//     }
//     return complete(c);
//   }

//   City* city;
//   int digx, digy, walkx, walky;
// };

// struct DigJob : Job {
//   DigJob(City* c, int dx_, int dy_, int wx_, int wy_)
//     : city(c), dx(dx_), dy(dy_), wx(wx_), wy(wy_) { }

//   virtual int description(char* buf, size_t n) const {
//     return snprintf(buf, n, "Digging");
//   }
//   virtual Department::Mask department() { return Department::FACILITIES; }
//   virtual Security::Mask security() { return Security::ALL; }

//   virtual AIState* get_script(Citizen* e) const {
//     return new DigAI(city, dx, dy, wx, wy);
//   }

//   City* city;
//   int dx, dy, wx, wy;
// };

void add_wall_dig_job(City* city, int x1, int y1, int digx, int digy) {
  Clearance c = { Security::ALL, Department::ALL };
  auto wall_cb = [=]() {
    return city->designs(digx, digy) & 1 && city->tile(digx, digy).type == Tile::wall;
  };
  auto dig_cb = [=]() { city->remove_wall(digx, digy); };

  AIScript* s1 = new SequenceAI{
    new ActivityAI(100),
    new_ifscript(wall_cb, new_callbackai(dig_cb))
  };

  AIScript* s2 = new SequenceAI{
    new PathAI(point(x1, y1)),
    new_ifscript(wall_cb, s1)
  };

  Job* job = new Job("Dig", c, s2);
  jobs.add_job(job);
}

vector<Room*> City::find_rooms(int x, int y) {
  vector<Room*> ret;
  for (auto r : rooms)
  if (r->contains(x, y))
    ret.push_back(r);
  return ret;
}

vector<Furniture*> City::find_furniture(int x, int y, int w, int h) {
  vector<Furniture*> ret;
  for (int i = x; i < x + w; ++i) {
    for (int j = y; j < y + h; ++j) {
      for (auto e : ent(i, j)) {
        if (e->has<Furniture>()) {
          ret.push_back(e->assert_get<Furniture>());
        }
      }
    }
  }
  return ret;
}

void City::toggle_dig_wall(int x, int y) {
  if (tile(x, y).type == Tile::wall) {
    designs(x, y) ^= 1; // Mark for digging
    if (designs(x, y) & 1) {
      for (auto o : offs) {
        if (tile(x + o.first, y + o.second).walkable()) {
          add_wall_dig_job(this, x + o.first, y + o.second, x, y);
          break;
        }
      }
    }
  }
}
void City::remove_wall(int x, int y) {
  if (tile(x, y).type != Tile::wall)
    return;
  designs(x, y) &= ~1;
  tile(x, y).type = Tile::ground;

  for (auto o : offs) {
    if (!(designs(x + o.first, y + o.second) & 1))
      continue;
    int facings = 0;
    for (auto o2 : offs)
    if (tile(x + o.first + o2.first, y + o.second + o2.second).walkable())
      ++facings;
    if (facings == 1)
      add_wall_dig_job(this, x, y, x + o.first, y + o.second);
  }
}
