#pragma once

struct Tile {
  enum TileKind : char {
    ground = '.',
    wall = '+'
  } type;

  char render() { return type; }

  bool walkable() { return type != wall; }
};
