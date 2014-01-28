#pragma once

#include "point.hpp"

extern struct ItemProperties hydroponics_table_properties;

struct Ent* make_hydroponics_table(const Point& p);
struct Ent* make_hydroponics_room(const Rect& r);
