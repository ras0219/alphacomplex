#pragma once

extern struct ItemProperties bread_properties;
extern struct ItemProperties lettuce_properties;

struct Point;

struct Ent* make_bread(const Point& p);
struct Ent* make_lettuce(const Point& p);
