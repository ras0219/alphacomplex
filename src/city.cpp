#include "components/ai/activityai.hpp"
#include "components/ai/callbackai.hpp"
#include "entities/citizen.hpp"
#include "city.hpp"
#include "defs.hpp"
#include "components/furniture.hpp"
#include "entities/garbage.hpp"
#include "components/ai/ifai.hpp"
#include "components/ai/ifai.hpp"
#include "log.hpp"
#include "components/ai/pathai.hpp"
#include "components/room.hpp"
#include "components/ai/sequenceai.hpp"
#include "windows.hpp"
#include "entities/workroom.hpp"
#include "joblist.hpp"

#include <stdexcept>

//const char* SentinelEntity::RAWNAME = "sentinel";

void City::resize(int x, int y) {
  xsz = x;
  ysz = y;
  furniture.resize(x, y);
  tiles.resize(x, y);
  ents.resize(x, y);
  designs.resize(x, y);
}

wistream& operator>>(wistream& is, City& c) {
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
      c.tile(i, y) = { (Tile::TileKind)s[i] };
      c.ent(i, y).clear();
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
        c.ent(i, j).insert(new_citizen({ &c, i, j }, Security::RED));
        c.tile(i, j).type = Tile::ground;
      } else if (ch == 'O') {
        // Citizen* e = new Citizen(i,j,Security::ORANGE, c);
        // e->insert_after(c.ent(i,j));
        c.ent(i, j).insert(new_citizen({ &c, i, j }, Security::ORANGE));
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
        c.rooms.push_back(make_workroom({ &c, i, j, w, h })->assert_get<Room>());
      }
    }
  }

  for (auto r : c.rooms)
    LOGGER::verbose() << r->r.w << 'x' << r->r.h << " Room @ " << r->r.x << ", " << r->r.y << endl;

  for (int j = 0; j < c.getYSize(); ++j) {
    for (int i = 0; i < c.getXSize(); ++i)
      LOGGER::verbose() << (char)c.tile(i, j).type;
    LOGGER::verbose() << '\n';
  }

  return is;
}

void add_wall_dig_job(City* city, int x1, int y1, int digx, int digy) {
  clearance c = { Security::ALL, Department::ALL };
  auto wall_cb = [=](AI*) {
    return city->designs(digx, digy) & 1 && city->tile(digx, digy).type == Tile::wall;
  };
  auto dig_cb = [=](AI*) { city->remove_wall(digx, digy); };

  auto s1 = make_shared<SequenceAI>();
  s1->add_task(make_shared<ActivityAI>(100));
  s1->add_task(new_ifscript(wall_cb, make_callbackai(dig_cb)));

  auto s2 = make_shared<SequenceAI>();
  s2->add_task(make_shared<PathAI>(point(x1, y1)));
  s2->add_task(new_ifscript(wall_cb, s1));

  std::shared_ptr<Job> job = make_shared<Job>("Dig", c, s2);
  jobs.add_job(job);
}

void City::add_room(Room* r) {
    assert(r != nullptr);
    assert(std::find(rooms.begin(), rooms.end(), r) == rooms.end());
    rooms.push_back(r);
}

void City::del_room(Room* r) {
    assert(r != nullptr);
    auto it = std::find(rooms.begin(), rooms.end(), r);
    assert(it != rooms.end());
    rooms.erase(it);
}
 

void City::add_furniture(Furniture* f) {
  assert(furniture(f->x(), f->y()) == nullptr);
  furniture(f->x(), f->y()) = f;
}

void City::del_furniture(Furniture* f) {
  assert(furniture(f->x(), f->y()) == f);
  furniture(f->x(), f->y()) = nullptr;
}

std::vector<Room*> City::find_rooms(int x, int y) {
  std::vector<Room*> ret;
  for (auto r : rooms)
  if (r->contains(x, y))
    ret.push_back(r);
  return ret;
}

std::vector<Furniture*> City::find_furniture(int x, int y, int w, int h) {
  std::vector<Furniture*> ret;
  for (int i = x; i < x + w; ++i) {
    for (int j = y; j < y + h; ++j) {
      if (furniture(i, j) != nullptr)
        ret.push_back(furniture(i, j));
    }
  }
  return ret;
}
std::vector<Furniture*> City::find_furniture(Rect r) {
    return find_furniture(r.x, r.y, r.w, r.h);
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
