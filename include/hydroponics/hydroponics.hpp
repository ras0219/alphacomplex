#pragma once

#include "point.hpp"

namespace ecs { struct Ent; }

namespace hydroponics {
    ecs::Ent* make_hydroponics_room(const Rect& r);
}
