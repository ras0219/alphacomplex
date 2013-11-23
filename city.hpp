#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include "graphics.hpp"
#include "tile.hpp"
#include "entity.hpp"
#include "component.hpp"
using namespace std;

struct SentinelEntity : Entity {
  SentinelEntity() : Entity(0) { }

  static const char* RAWNAME;
  virtual const char* rawname() const { return RAWNAME; }
};

extern struct City city;

struct City : Component {
  int xsz;
  int ysz;
  vector<Tile> tiles;
  vector<SentinelEntity> ents;
  vector<struct Room*> rooms;

  inline int getXSize() const { return xsz; }
  inline int getYSize() const { return ysz; }

  inline Tile tile(int x, int y) const { return tiles[xsz*y + x]; }
  inline Tile& tile(int x, int y) { return tiles[xsz*y + x]; }

  inline const SentinelEntity* ent(int x, int y) const { return &ents[xsz*y + x]; }
  inline SentinelEntity* ent(int x, int y) { return &ents[xsz*y + x]; }

  inline bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  City() : xsz(0), ysz(0), tiles(), ents() {}
  City(int x, int y) : xsz(x), ysz(y), tiles(x*y), ents(x*y) {}

  virtual void render(Graphics&);

  struct Room* find_room(const char*);
};

istream& operator>>(istream& is, City& city);
