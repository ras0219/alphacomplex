#pragma once

#include "point.hpp"

extern struct ItemProperties filingcabinet_properties;

struct Ent* make_filestorage(const Rect& r);
struct Ent* make_filingcabinet(const Point& p);