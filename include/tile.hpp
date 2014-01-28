#pragma once

struct Tile {
  enum TileKind : char {
    ground = '.',
    wall = '+',
    door = '*',
    //furniture = 'P',
    engineering = 'S',
    foodsupply = 'W',
    infirmary = '.',
    filestorage = 'F'
  } type;

  char render() { return type; }

  bool walkable() { return type != wall; }
};
