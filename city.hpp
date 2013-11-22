#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include "graphics.hpp"
#include "tile.hpp"
#include "entity.hpp"

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

  int getXSize() const { return xsz; }
  int getYSize() const { return ysz; }

  Tile tile(int x, int y) const { return tiles[xsz*y + x]; }
  Tile& tile(int x, int y) { return tiles[xsz*y + x]; }

  const SentinelEntity* ent(int x, int y) const { return &ents[xsz*y + x]; }
  SentinelEntity* ent(int x, int y) { return &ents[xsz*y + x]; }

  inline bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  City() : xsz(0), ysz(0), tiles(), ents() {}
  City(int x, int y) : xsz(x), ysz(y), tiles(x*y), ents(x*y) {}

  void render(Graphics& g) {
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

};

istream& operator>>(istream& is, City& city);
