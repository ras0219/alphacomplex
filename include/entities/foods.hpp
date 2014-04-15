#pragma once

namespace item {
    struct ItemProperties;
}
namespace ecs {
    struct Ent;
}

extern item::ItemProperties bread_properties;
extern item::ItemProperties lettuce_properties;

struct Point;

ecs::Ent* make_bread(const Point& p);
ecs::Ent* make_lettuce(const Point& p);
