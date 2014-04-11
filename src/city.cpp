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

template<class T>
Overlay<T>::Overlay(int x, int y) : xsz(x), ysz(y), data(xsz*ysz) {}

template<class T>
int Overlay<T>::getXSize() const
{
  return xsz;
}

template<class T>
int Overlay<T>::getYSize() const
{
  return ysz;
}

template <class T>
bool Overlay<T>::check(int x, int y) const {
  return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
}

template <class T>
T& Overlay<T>::get(int x, int y) { return data[xsz*y + x]; }

template <class T>
const T& Overlay<T>::get(int x, int y) const
{
  return data[xsz*y + x];
}

template <class T>
T& Overlay<T>::operator()(int x, int y)
{
  return get(x,y);
}
template <class T>
const T& Overlay<T>::operator()(int x, int y) const
{
  return get(x,y);
}

template <class T> void Overlay<T>::resize(int x, int y) { xsz = x; ysz = y; data.resize(x*y); }

int City::getXSize() const { return xsz; }
int City::getYSize() const { return ysz; }

Tile City::tile(int x, int y) const { return tiles.data[xsz*y + x]; }
Tile& City::tile(int x, int y) { return tiles.data[xsz*y + x]; }

const ents_t& City::ent(int x, int y) const { return ents.data[xsz*y + x]; }
ents_t& City::ent(int x, int y) { return ents.data[xsz*y + x]; }

void City::add_ent(int x, int y, Ent* e) {
  ent(x, y).insert(e);
}
void City::del_ent(int x, int y, Ent* e) {
  ent(x, y).erase(e);
}

bool City::check(int x, int y) const {
  return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
}

City::City(int x, int y) : xsz(x), ysz(y),
			   tiles(x,y),
			   ents(x,y),
			   designs(x,y),
			   furniture(x,y){}
City::City(struct CityProperties const& cityP) :
  xsz(cityP.width), ysz(cityP.height),
  tiles(xsz, ysz),
  ents(xsz, ysz),
  designs(xsz, ysz),
  furniture(xsz, ysz)
{
  randomgen(*this, cityP);
}

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
  clearance c = { Security::ALL, Department::ALL };
  auto wall_cb = [=](AI*) {
    return city->designs(digx, digy) & 1 && city->tile(digx, digy).type == Tile::wall;
  };
  auto dig_cb = [=](AI*) { city->remove_wall(digx, digy); };

  SequenceAI::ptr s1 = make_shared<SequenceAI>();
  s1->add_task(make_shared<ActivityAI>(100));
  s1->add_task(new_ifscript(wall_cb, make_callbackai(dig_cb)));

  SequenceAI::ptr s2 = make_shared<SequenceAI>();
  s2->add_task(make_shared<PathAI>(point(x1, y1)));
  s2->add_task(new_ifscript(wall_cb, s1));

  std::shared_ptr<Job> job = make_shared<Job>("Dig", c, s2);
  jobs.add_job(job);
}

void City::add_room(Room* r) {
  rooms.push_back(r);

  auto v = find_furniture(r->r.x, r->r.y, r->r.w, r->r.h);
  r->furniture = Room::set_t(v.begin(), v.end());
  for (auto f : r->furniture)
    f->rooms.insert(r);
}

void City::del_room(Room* r) {
  auto it = std::find(rooms.begin(), rooms.end(), r);
  assert(it != rooms.end());
  rooms.erase(it);

  for (auto f : r->furniture)
    f->rooms.erase(r);
  r->furniture.clear();
}
 

void City::add_furniture(Furniture* f) {
  assert(furniture(f->x(), f->y()) == nullptr);
  furniture(f->x(), f->y()) = f;

  std::vector<struct Room*> rs = find_rooms(f->x(), f->y());
  f->rooms = Furniture::set_t(rs.begin(), rs.end());

  for (auto r : f->rooms)
    r->furniture.insert(f);
}

void City::del_furniture(Furniture* f) {
  assert(furniture(f->x(), f->y()) == f);
  furniture(f->x(), f->y()) = nullptr;

  for (auto r : f->rooms)
    r->furniture.erase(f);
  f->rooms.clear();
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
