#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <time.h>

#include "defs.hpp"
#include "graphics.hpp"
#include "tile.hpp"

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

struct CityProperties
{
public:
    CityProperties(){}
    CityProperties(int iHeight, int iWidth)
    {
        height = iHeight;
        width = iWidth;
        seed = (int) time(NULL);
        top = 0;
        left = 0;
        bottom = height - 1;
        right = width - 1;
    }

    CityProperties(int iHeight, int iWidth, int iSeed)
    {
        height = iHeight;
        width = iWidth;
        seed = iSeed;
        top = 0;
        left = 0;
        bottom = height - 1;
        right = width - 1;
    }

    int height, width, seed, top, left, bottom, right;
};

enum RoomTypes
{
    Regular,
    FoodSupply,
    FileStorage,
    Infirmary,
    Engineering,
    RoomTypesCount
};

struct RoomProperties
{
    int height, width, seed, top, left, bottom, right;
    RoomTypes roomType;
    bool isCompound, floorOnly;
    bool doorN, doorE, doorW, doorS;
    RoomProperties(int iTop, int iLeft, int iHeight, int iWidth)
    {
        top = iTop;
        left = iLeft;
        bottom = iTop + iHeight - 1;
        right = iLeft + iWidth - 1;
        height = iHeight;
        width = iWidth;

        // randomly determine the room type
        roomType = (RoomTypes) (rand() % RoomTypes::RoomTypesCount);

        // 2/3 chance that the room has smaller rooms inside it
        isCompound = ((rand() % 3) != 0);

        // is the room purely floor
        floorOnly = false;
        if ((height < 3) || (width < 3))
        {
            floorOnly = true;
        }

        // assume you can put doors on all sides
        doorN = true;
        doorS = true;
        doorE = true;
        doorW = true;

        // if the wall size is too small then don't put
        // the door on that wall
        if (height < 3)
        {
            doorE = false;
            doorW = false;
        }

        // if the wall size is too small then don't put
        // the door on that wall
        if (width < 3)
        {
            doorN = false;
            doorS = false;
        }
    }
};

class MathHelper
{
public:
    static int GetFactor(int p)
    {
        int factor = (p / 2);
        for (; factor > 2; factor--)
        {
            if ((p % factor) == 0)
                return factor;
        }
        return p;
    }
};

#define CITY_GENERATION_DEPTH 2
#define ROOM_MIN_SEGMENTS 2
#define ROOM_MAX_SEGMENTS 6

struct City {
  using ents_t = unordered_set<Ent*>;

  int xsz;
  int ysz;
  vector<Tile> tiles;
  vector< ents_t > ents;
  vector<struct Room*> rooms;
  Overlay<char> designs;

  inline int getXSize() const { return xsz; }
  inline int getYSize() const { return ysz; }

  inline Tile tile(int x, int y) const { return tiles[xsz*y + x]; }
  inline Tile& tile(int x, int y) { return tiles[xsz*y + x]; }

  inline const ents_t& ent(int x, int y) const { return ents[xsz*y + x]; }
  inline ents_t& ent(int x, int y) { return ents[xsz*y + x]; }

  inline void add_ent(int x, int y, Ent* e) {
    ent(x, y).insert(e);
  }
  inline void del_ent(int x, int y, Ent* e) {
    ent(x, y).erase(e);
  }

  /// Insert a room. Fast Amortized O(1).
  /// Precondition: Room has not already been added (not checked).
  inline void add_room(struct Room* r) {
    rooms.push_back(r);
  }

  /// Delete a room by reference. Fast O(R).
  inline void del_room(struct Room* r) {
    auto it = std::find(rooms.begin(), rooms.end(), r);
    assert(it != rooms.end());
    rooms.erase(it);
  }

  /// Find all furniture within a square. Fast O(w*h + E) where E is entities within (x,y,w,h).
  vector<struct Furniture*> find_furniture(int x, int y, int w, int h);

  /// Find all rooms containing a square. Fast O(R).
  vector<struct Room*> find_rooms(int x, int y);

  /// Check that x and y are within the current city
  inline bool check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
  }

  /// Toggle whether the wall at (x, y) should have a dig job.
  void toggle_dig_wall(int x, int y);

  /// Replace a wall at (x, y) with empty ground.
  void remove_wall(int x, int y);

  /// Resize the city. This should not be called after initialization.
  void resize(int x, int y);

  City() : xsz(0), ysz(0), designs(0, 0) {}
  City(int x, int y) : xsz(x), ysz(y), tiles(x*y),
                       ents(x*y),
                       designs(xsz, ysz) {}

  /// Constructor: uses the city properties to generate a city
  City(const CityProperties& cityP) :
      xsz(cityP.width), ysz(cityP.height),
      tiles(xsz*ysz),
      ents(xsz*ysz),
      designs(xsz, ysz),
      city_properties(cityP)
  {
    // set seed for the city generator
    srand(cityP.seed);

    // assume the entire city area is just ground
    for (int i = 0; i < ysz; ++i) 
    {
        for (int j = 0; j < xsz; ++j)
        {
            tiles.push_back({ Tile::TileKind::ground });
        }
        ents.emplace_back();
    }

    // recursively generate city
    // TODO: may be we can move this to initialize()
    generate(CITY_GENERATION_DEPTH);
  }
  CityProperties city_properties;

  struct Room* find_room(const char*);

private:
  // generates random int value bound by min and max
  // TODO: move to a helper file
  int randNext() { return rand(); }
  int randNext(int max) 
  {
    if (max == 0) return 0;
    return rand() % max; 
  }
  int randNext(int min, int max) 
  {
    if (min >= max) return min;
    return min + (rand() % (max - min)); 
  }

  // Recursive city generator. Accepts 'depth' to determine 
  // the segmentation granularity.
  void generate(int depth)
  {
    // determine the number of segments to cut vertically and horizontally
    int vRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);
    int hRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);

    // determine the size of each segment
    int vSize = ysz / vRooms;
    int hSize = xsz / hRooms;

    // often the factorization might not be clean
    // so determine where the vertical oversized segment should go.
    int *vSegments = new int[vRooms];
    for (int i = 0; i < vRooms; i++)
      vSegments[i] = vSize;
    int vIndex = randNext(0, vRooms);
    vSegments[vIndex] = vSize + ysz - (vSize * vRooms);

    // often the factorization might not be clean
    // so determine where the horizontal oversized segment should go.
    int *hSegments = new int[hRooms];
    for (int i = 0; i < hRooms; i++)
      hSegments[i] = hSize;
    int hIndex = randNext(0, hRooms);
    hSegments[hIndex] = hSize + xsz - (hSize * hRooms);

    // Start placing rooms recursively
    for (int i = 0, top = 0; i < vRooms; i++)
    {
      for (int j = 0, left = 0; j < hRooms; j++)
      {
        // determine the coordinates of the room
        int height = vSegments[i], width = hSegments[j];
        int actualTop = top, actualLeft = left;

        // this is to avoid double walling each room
        // if removed each room will sit inside a previous bound
        // hence adding a wall for each level of depth
        if (i != 0)
        {
          actualTop = top - 1;
          height += 1;
        }
        if (j != 0)
        {
          actualLeft = left - 1;
          width += 1;
        }

        // create room properties, this determines what kind of room, if it has doors
        // where the doors are, orientation, etc
        RoomProperties rp(actualTop, actualLeft, height, width);

        // determine if the room is a boundry for smaller rooms
        if (rp.isCompound)
        {
          // build more rooms inside the boundary
          generate(rp, depth - 1);
        }
        else
        {
          // construct the room using the room properties
          build_room(rp);
        }
        
        left += hSegments[j];
      }

      top += vSegments[i];
    }

    // construct the perimeter wall
    build_perimeter_wall();

    // add RED entities at a random position
    add_entities(Security::Mask::RED, 2, 4);

    // add ORANGE entities at a random position
    add_entities(Security::Mask::ORANGE, 1, 1);

    // change doors to ground
    finalize_doors();

    // find room locations
    // TODO: this can be removed( see comment on method) 
    find_rooms();

    delete[] vSegments;
    delete[] hSegments;
  }

  // Recursive city generator. Accepts 'depth' to determine 
  // the segmentation granularity. This version uses room properties to 
  // perform segmentation.
  void generate(const RoomProperties& roomProperties, int depth)
  {
    // if the depth is reached draw floor 
    if (depth <= 0)
    {
      build_floor(roomProperties);
      return;
    }

    // determine the number of segments to cut vertically and horizontally
    int vRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);
    int hRooms = randNext(ROOM_MIN_SEGMENTS, ROOM_MAX_SEGMENTS);

    // determine the size of each segment
    int vSize = roomProperties.height / vRooms;
    int hSize = roomProperties.width / hRooms;

    // if the size is too small return
    if ((vSize <= 0) || (hSize <= 0))
    {
      return;
    }

    // often the factorization might not be clean
    // so determine where the vertical oversized segment should go.
    int *vSegments = new int[vRooms];
    for (int i = 0; i < vRooms; i++)
      vSegments[i] = vSize;
    int vIndex = randNext(0, vRooms);
    vSegments[vIndex] = vSize + roomProperties.height - (vSize * vRooms);

    // often the factorization might not be clean
    // so determine where the horizontal oversized segment should go.
    int *hSegments = new int[hRooms];
    for (int i = 0; i < hRooms; i++)
      hSegments[i] = hSize;
    int hIndex = randNext(0, hRooms);
    hSegments[hIndex] = hSize + roomProperties.width - (hSize * hRooms);

    // Start placing rooms recursively
    for (int i = 0, top = roomProperties.top; i < vRooms; i++)
    {
      for (int j = 0, left = roomProperties.left; j < hRooms; j++)
      {
        // determine the coordinates of the room
        int height = vSegments[i], width = hSegments[j];
        int actualTop = top, actualLeft = left;

        // this is to avoid double walling each room
        // if removed each room will sit inside a previous bound
        // hence adding a wall for each level of depth
        if (i != 0)
        {
          actualTop = top - 1;
          height += 1;
        }
        if (j != 0)
        {
          actualLeft = left - 1;
          width += 1;
        }

        // create room properties, this determines what kind of room, if it has doors
        // where the doors are, orientation, etc
        RoomProperties rp(actualTop, actualLeft, height, width);

        // determine if the room is a boundry for smaller rooms
        if (rp.isCompound)
        {
          // build more rooms inside the boundary
          generate(rp, depth - 1);
        }
        else
        {
          // construct the room using the room properties
          build_room(rp);
        }

        left += hSegments[j];
      }
      top += vSegments[i];
    }

    delete[] vSegments;
    delete[] hSegments;
  }

  // adds a random number of entities of a specified type
  void add_entities(Security::Mask entity, int min, int max);

  // construct a room
  void build_room(const RoomProperties& rp)
  {
    // if the room is just a floor build it
    if (rp.floorOnly)
    {
      build_floor(rp);
      return;
    }

    // construct a specific room.
    switch (rp.roomType)
    {
    case RoomTypes::Regular:
      build_regular(rp);
      build_doors(rp);
      break;
    case RoomTypes::FileStorage:
      build_file_storage(rp);
      build_doors(rp);
      break;
    case RoomTypes::Engineering:
      build_engineering(rp);
      build_doors(rp);
      break;
    case RoomTypes::FoodSupply:
      build_food_supply(rp);
      build_doors(rp);
      break;
    case RoomTypes::Infirmary:
      build_infirmary(rp);
      build_doors(rp);
      break;
    default:
      break;
    }
  }
  void build_regular(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if (tile(j, i).type != Tile::TileKind::door)
        {
          if ((i == rp.top) || (i == rp.bottom) ||
            (j == rp.left) || (j == rp.right))
          {
            tile(j, i).type = Tile::TileKind::wall;
          }
          else
          {
            tile(j, i).type = Tile::TileKind::ground;
          }
        }
      }
    }
  }

  void build_file_storage(const RoomProperties& rp)
  {
    build_regular(rp);

    if ((rp.width >= 5) || (rp.height >= 5))
    {
      int top = rp.top + 2;
      int bottom = rp.bottom - 2;
      int left = rp.left + 2;
      int right = rp.right - 2;

      if (rp.height > rp.width)
      {
        int factor = MathHelper::GetFactor(rp.height - 4);
        if ((rp.height - 4) <= 8)
        {
          factor = rp.height;
        }

        for (int i = top; i <= bottom; i++)
        {
          for (int j = left; j <= right; j += 2)
          {
            if ((tile(j, i).type == Tile::TileKind::ground) && ((i % factor) != 0))
            {
              tile(j, i).type = Tile::TileKind::furniture;
            }
          }
        }
      }
      else
      {
        int factor = MathHelper::GetFactor(rp.width - 4);
        if ((rp.width - 4) <= 8)
        {
          factor = rp.width;
        }

        for (int i = top; i <= bottom; i += 2)
        {
          for (int j = left; j <= right; j++)
          {
            if ((tile(j, i).type == Tile::TileKind::ground) && ((j % factor) != 0))
            {
              tile(j, i).type = Tile::TileKind::furniture;
            }
          }
        }
      }
    }
  }

  void build_engineering(const RoomProperties& rp)
  {
    build_regular(rp);

    if (((rp.width >= 5) && (rp.height >= 8)) ||
      ((rp.width >= 8) && (rp.height >= 5)))
    {
      int top = rp.top + 2;
      int bottom = rp.bottom - 2;
      int left = rp.left + 2;
      int right = rp.right - 2;

      if (rp.height > rp.width)
      {
        if ((rp.height % 2) == 0)
        {
          for (int i = top; i <= (top + 1); i++)
          {
            for (int j = left; j <= right; j++)
            {
              if (tile(j, i).type == Tile::TileKind::ground)
              {
                tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        }
        else
        {
          for (int i = (bottom - 1); i <= bottom; i++)
          {
            for (int j = left; j <= right; j++)
            {
              if (tile(j, i).type == Tile::TileKind::ground)
              {
                tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        }
      }
      else
      {
        if ((rp.width % 2) == 0)
        {
          for (int i = top; i <= bottom; i++)
          {
            for (int j = left; j <= (left + 1); j++)
            {
              if (tile(j, i).type == Tile::TileKind::ground)
              {
                tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        }
        else
        {
          for (int i = top; i <= bottom; i++)
          {
            for (int j = (right - 1); j <= right; j++)
            {
              if (tile(j, i).type == Tile::TileKind::ground)
              {
                tile(j, i).type = Tile::TileKind::engineering;
              }
            }
          }
        }
      }
    }
  }

  void build_food_supply(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if (tile(j, i).type != Tile::TileKind::door)
        {
          if ((i == rp.top) || (i == rp.bottom) ||
            (j == rp.left) || (j == rp.right))
          {
            tile(j, i).type = Tile::TileKind::wall;
          }
          else
          {
            tile(j, i).type = Tile::TileKind::foodsupply;
          }
        }
      }
    }
  }

  void build_infirmary(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if (tile(j, i).type != Tile::TileKind::door)
        {
          if ((i == rp.top) || (i == rp.bottom) ||
            (j == rp.left) || (j == rp.right))
          {
            tile(j, i).type = Tile::TileKind::wall;
          }
          else
          {
            tile(j, i).type = Tile::TileKind::infirmary;
          }
        }
      }
    }
  }

  // mark anything that is not already a wall or door as floor
  void build_floor(const RoomProperties& rp)
  {
    for (int i = rp.top; i <= rp.bottom; i++)
    {
      for (int j = rp.left; j <= rp.right; j++)
      {
        if ((tile(j,i).type != Tile::TileKind::door) &&
          (tile(j, i).type != Tile::TileKind::wall))
        {
          tile(j, i).type = Tile::TileKind::ground;
        }
      }
    }
  }

  // Determines where the door has to be placed
  void build_doors(const RoomProperties& rp)
  {
    if (rp.doorN)
    {
      int i = (rp.width % 2 == 0) ? rp.width / 2 : rp.width / 2 + 1;
      tile(rp.left + i, rp.top).type = Tile::TileKind::door;
    }

    if (rp.doorS)
    {
      int i = (rp.width % 2 == 0) ? rp.width / 2 : rp.width / 2 + 1;
      tile(rp.left + i, rp.bottom).type = Tile::TileKind::door;
    }

    if (rp.doorE)
    {
      int i = (rp.height % 2 == 0) ? rp.height / 2 : rp.height / 2 + 1;
      tile(rp.right, rp.top + i).type = Tile::TileKind::door;
    }
    if (rp.doorW)
    {
      int i = (rp.height % 2 == 0) ? rp.height / 2 : rp.height / 2 + 1;
      tile(rp.left, rp.top + i).type = Tile::TileKind::door;
    }
  }

  // Generates a perimeter wall for the entire city
  // TODO: this can be optimized to speed up city generation
  void build_perimeter_wall()
  {
    for (int i = city_properties.top; i < city_properties.height; i++)
    {
      for (int j = city_properties.left; j < city_properties.width; j++)
      {
        if ((i == 0) || (i == city_properties.bottom) ||
          (j == 0) || (j == city_properties.right))
        {
          tile(j, i).type = Tile::TileKind::wall;
        }
      }
    }
  }

  // While generating rooms the doors are represented by 
  // TileKind::door this method replaces them with TileKind::ground
  void finalize_doors()
  {
    for (int j = city_properties.top; j < city_properties.height; j++)
    {
      for (int i = city_properties.left; i < city_properties.width; i++)
      {

        if (tile(i, j).type == Tile::TileKind::door)
        {
          tile(i, j).type = Tile::TileKind::ground;
        }
      }
    }
  }

  // Finds the rooms in the generated city
  // TODO: this can be removed since rooms are
  // procedurally generated we don't have to find them
  void find_rooms();
};

wistream& operator>>(wistream& is, City& city);



