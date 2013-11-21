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

  static constexpr const char* RAWNAME = "sentinel";
  virtual const char* rawname() const { return RAWNAME; }

  virtual char render() const { return '\0'; }
};

struct City {
  int xsz;
  int ysz;
  vector<Tile> tiles;
  vector<SentinelEntity> ents;

  int getXSize() const { return xsz; }
  int getYSize() const { return ysz; }

  Tile tile(int x, int y) const { return tiles[ysz*y + x]; }
  Tile& tile(int x, int y) { return tiles[ysz*y + x]; }

  const SentinelEntity* ent(int x, int y) const { return &ents[ysz*y + x]; }
  SentinelEntity* ent(int x, int y) { return &ents[ysz*y + x]; }

  inline bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  City(int x, int y) : xsz(x), ysz(y), tiles(x*y), ents(x*y) {}

  inline void render(Graphics& g) {
    for (int y=0;y<12;++y)
      for (int x=0;x<12;++x) {
        assert(ent(x,y)->rawname() == SentinelEntity::RAWNAME);
        if (ent(x,y)->next != nullptr) {
          ent(x,y)->next->render(g);
        } else {
          g.putChar(x, y, tile(x,y).type);
        }
      }
  }

};
extern City city;

istream& operator>>(istream& is, City& city);
