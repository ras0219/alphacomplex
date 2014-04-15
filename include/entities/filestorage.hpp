#pragma once

#include "point.hpp"

namespace item {
    struct ItemProperties;
}
namespace ecs {
    struct Ent;
}

extern item::ItemProperties filingcabinet_properties;

ecs::Ent* make_filestorage(const Rect& r);
ecs::Ent* make_filingcabinet(const Point& p);