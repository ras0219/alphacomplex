#pragma once

//#include "point.hpp"

extern struct ItemProperties bread_properties;
extern struct ItemProperties lettuce_properties;

class Point;

struct Ent* make_bread(const Point& p);
struct Ent* make_lettuce(const Point& p);
