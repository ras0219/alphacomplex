#pragma once

#include "components/component.hpp"
#include "entities/subsystem.hpp"
#include "position.hpp"

struct City;

namespace ecs {
    struct Ent;
}

namespace movement {

    struct Movable : ecs::ComponentCRTP<ecs::Component::Movable, Movable> {
        Movable(Point p) : pos(p) { }

        inline City& city() { return *pos.city; }
        inline int x() const { return pos.x; }
        inline int y() const { return pos.y; }
        inline point as_point() const { return pos.as_point(); }

        inline void set(int x, int y) {
            pos.x = x;
            pos.y = y;
        }
        inline void set(point p) {
            pos.x = p.first;
            pos.y = p.second;
        }

        Point pos;
    };

    struct MovableSystem : ecs::SubSystem<MovableSystem, Movable, Position> {
        inline void update_item(ecs::Ent*, Movable* m, Position* pc) {
            pc->move(m->pos);
        }

        static ecs::CRTPSystemFactory<MovableSystem> factory;
    };

}