#pragma once

struct Rect;

namespace ecs {
    struct Ent;
}

namespace storage {
    ecs::Ent* make_storageroom(const Rect&);
}