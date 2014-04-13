#pragma once

#include <utility>

typedef std::pair<int, int> point;

struct Point {
  point as_point() const;

  struct City* city;
  int x, y;

  bool operator==(const Point& o);
  bool operator!=(const Point& o);
};

inline bool operator==(const point& p1, const point& p2) {
    return p1.first == p2.first && p1.second == p2.second;
}
inline bool operator!=(const point& p1, const point& p2) {
    return !(p1 == p2);
}

struct rect {
  int x, y, w, h;

  bool overlaps(rect o);
};

struct Rect {
  rect as_rect() const;

  struct City* city;
  int x, y, w, h;
};
