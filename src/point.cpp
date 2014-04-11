#include "point.hpp"

point Point::as_point() const {
  return {x,y};
}

bool Point::operator==(const Point& o) {
  return o.city == city && o.x == x && o.y == y;
}

bool Point::operator!=(const Point& o) {
  return !(*this == o);
}

bool rect::overlaps(rect o) {
    return x < (o.x + o.w) && (x + w) > o.x
      && y < (o.y + o.h) && (y + h) > o.y;
}

rect Rect::as_rect() const { return{ x, y, w, h }; }


