#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <set>
#include <algorithm>

#include "defs.hpp"
#include "graphics.hpp"
#include "tile.hpp"
#include "component.hpp"

using namespace std;

struct Ent;

template<class T>
struct Overlay {
  int getXSize() const { return xsz; }
  int getYSize() const { return ysz; }

  bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  T& get(int x, int y) { return data[xsz*y + x]; }
  const T& get(int x, int y) const { return data[xsz*y + x]; }

  T& operator()(int x, int y) { return get(x,y); }
  const T& operator()(int x, int y) const { return get(x,y); }

  void resize(int x, int y) { xsz = x; ysz = y; data.resize(x*y); }

  Overlay(int x, int y) : xsz(x), ysz(y), data(xsz*ysz) {}

  int xsz;
  int ysz;
  vector<T> data;
};

struct City {
  int xsz;
  int ysz;
  vector<Tile> tiles;
  vector< set<Ent*> > ents;
  vector<struct Room*> rooms;
  Overlay<char> designs;

  inline int getXSize() const { return xsz; }
  inline int getYSize() const { return ysz; }

  inline Tile tile(int x, int y) const { return tiles[xsz*y + x]; }
  inline Tile& tile(int x, int y) { return tiles[xsz*y + x]; }

  inline const set<Ent*>& ent(int x, int y) const { return ents[xsz*y + x]; }
  inline set<Ent*>& ent(int x, int y) { return ents[xsz*y + x]; }

  inline void add_ent(int x, int y, Ent* e) {
    ent(x, y).insert(e);
  }
  inline void del_ent(int x, int y, Ent* e) {
    ent(x, y).erase(e);
  }

  inline void add_room(struct Room* r) {
    rooms.push_back(r);
  }
  inline void del_room(struct Room* r) {
    auto it = std::find(rooms.begin(), rooms.end(), r);
    assert(it != rooms.end());
    rooms.erase(it);
  }

  vector<struct Furniture*> find_furniture(int x, int y, int w, int h);
  vector<struct Room*> find_rooms(int x, int y);

  inline bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  void toggle_dig_wall(int x, int y);
  void remove_wall(int x, int y);

  inline void resize(int x, int y) { xsz = x; ysz = y; designs.resize(x, y); }

  City() : xsz(0), ysz(0), designs(0, 0) {}
  City(int x, int y) : xsz(x), ysz(y), tiles(x*y),
                       ents(x*y),
                       designs(xsz, ysz) {}

  struct Room* find_room(const char*);
};

istream& operator>>(istream& is, City& city);

struct Position {
  inline point as_point() const { return {x, y}; }

  int x, y;
  City* city;
};

