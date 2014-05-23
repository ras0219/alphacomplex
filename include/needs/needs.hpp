#pragma once

#include "components/component.hpp"

namespace needs {

    struct Needs : ecs::ComponentCRTP<ecs::Component::Needs, Needs> {
        int food = 100;
        int max_food = 200;

        int sleep = 200;
        int max_sleep = 200;

        int happy = 200;
        int max_happy = 200;
    };

}