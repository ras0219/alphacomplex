#pragma once

#include <vector>
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <algorithm>

#include "defs.hpp"
#include "tile.hpp"
#include "citygen.hpp"

using namespace std;

struct Ent;

template<class T>
struct Overlay {
  int getXSize() const;
  int getYSize() const;

  bool check(int x, int y) const ;

  T& get(int x, int y) ;
  const T& get(int x, int y) const;

  T& operator()(int x, int y);
  const T& operator()(int x, int y) const;

  void resize(int x, int y);

  Overlay(int x, int y);

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

  int getXSize() const;
  int getYSize() const;

  Tile tile(int x, int y) const;
  Tile& tile(int x, int y);

  const ents_t& ent(int x, int y) const;
  ents_t& ent(int x, int y);

  void add_ent(int x, int y, Ent* e);
  void del_ent(int x, int y, Ent* e);

  /// Insert a room. Fast Amortized O(1).
  /// @pre Room has not already been added (asserted).
  /// @pre Room != nullptr (asserted).
  void add_room(struct Room* r);

  /// Delete a room by pointer. Fast O(R).
  /// @pre Room has been added (asserted).
  /// @pre Room != nullptr (asserted).
  void del_room(struct Room* r);

  /// Place a piece of furniture. Fast O(1).
  /// @pre Location has no furniture.
  /// @post Location has furniture f.
  void add_furniture(struct Furniture* f);

  /// Remove a piece of furniture. Fast O(1).
  /// @pre Location has furniture f.
  /// @post Location has no furniture.
  void del_furniture(struct Furniture* f);

  /// Find all furniture within a rectangle. Fast O(w*h).
  /// @return List of found furniture.
  vector<struct Furniture*> find_furniture(int x, int y, int w, int h);

  /// Convenience overload for find_furniture(x,y,w,h).
  /// @pre Rect r is associated with this city (not checked).
  /// @return List of found furniture
  vector<struct Furniture*> find_furniture(Rect r);

  /// Find all rooms containing a point. Fast O(R).
  /// @return List of found rooms
  vector<struct Room*> find_rooms(int x, int y);

  /// Check that x and y are within the current city
  bool check(int x, int y) const;

  /// Toggle whether the wall at (x, y) should have a dig job.
  void toggle_dig_wall(int x, int y);

  /// Replace a wall at (x, y) with empty ground.
  void remove_wall(int x, int y);

  /// Resize the city. This should not be called after initialization.
  void resize(int x, int y);

  /// Selects a random walkable tile in the city
  point random_point();

  City(int x, int y);

  /// Constructor: uses the city properties to generate a city
  City(struct CityProperties const& cityP);
  
};

wistream& operator>>(wistream& is, City& city);
