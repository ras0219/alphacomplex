#pragma once

#include <vector>
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <algorithm>

#include "defs.hpp"
#include "graphics.hpp"
#include "tile.hpp"
#include "citygen.hpp"

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
  std::vector<T> data;
};

struct City {
  using ents_t = unordered_set<Ent*>;

  int xsz;
  int ysz;
  Overlay<Tile> tiles;
  Overlay<ents_t> ents;
  Overlay<char> designs;
  Overlay<struct Furniture*> furniture;

  vector<struct Room*> rooms;

  inline int getXSize() const { return xsz; }
  inline int getYSize() const { return ysz; }

  inline Tile tile(int x, int y) const { return tiles.data[xsz*y + x]; }
  inline Tile& tile(int x, int y) { return tiles.data[xsz*y + x]; }

  inline const ents_t& ent(int x, int y) const { return ents.data[xsz*y + x]; }
  inline ents_t& ent(int x, int y) { return ents.data[xsz*y + x]; }

  inline void add_ent(int x, int y, Ent* e) {
    ent(x, y).insert(e);
  }
  inline void del_ent(int x, int y, Ent* e) {
    ent(x, y).erase(e);
  }

  /// Insert a room. Fast Amortized O(1).
  /// Precondition: Room has not already been added (asserted).
  /// Precondition: Room != nullptr (asserted).
  void add_room(struct Room* r);

  /// Delete a room by pointer. Fast O(R).
  /// Precondition: Room has been added (asserted).
  /// Precondition: Room != nullptr (asserted).
  void del_room(struct Room* r);

  /// Place a piece of furniture. Fast O(1).
  /// Precondition: Location has no furniture.
  /// Postcondition: Location has furniture f.
  void add_furniture(struct Furniture* f);

  /// Remove a piece of furniture. Fast O(1).
  /// Precondition: Location has furniture f.
  /// Postcondition: Location has no furniture.
  void del_furniture(struct Furniture* f);

  /// Find all furniture within a rectangle. Fast O(w*h).
  vector<struct Furniture*> find_furniture(int x, int y, int w, int h);

  /// Convenience overload for find_furniture(x,y,w,h).
  /// Precondition: Rect r is associated with this city (not checked).
  vector<struct Furniture*> find_furniture(Rect r);

  /// Find all rooms containing a point. Fast O(R).
  vector<struct Room*> find_rooms(int x, int y);

  /// Check that x and y are within the current city.
  inline bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  /// Toggle whether the wall at (x, y) should have a dig job.
  void toggle_dig_wall(int x, int y);

  /// Replace a wall at (x, y) with empty ground.
  void remove_wall(int x, int y);

  /// Resize the city. This should not be called after initialization.
  void resize(int x, int y);

  City(int x, int y) : xsz(x), ysz(y), tiles(x, y),
                       ents(x, y),
                       designs(x, y),
                       furniture(x, y) {}

  /// Constructor: uses the city properties to generate a city
  City(struct CityProperties const& cityP) :
      xsz(cityP.width), ysz(cityP.height),
      tiles(xsz, ysz),
      ents(xsz, ysz),
      designs(xsz, ysz),
      furniture(xsz, ysz)
  {
    randomgen(*this, cityP);
  }
};

wistream& operator>>(wistream& is, City& city);
