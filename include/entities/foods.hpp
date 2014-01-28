#pragma once

#include "point.hpp"

extern struct ItemProperties bread_properties;
extern struct ItemProperties lettuce_properties;

struct Ent* make_bread(const Point& p);
struct Ent* make_lettuce(const Point& p);