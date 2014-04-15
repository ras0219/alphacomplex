#pragma once

#include "entities/entity.hpp"
#include "ai.hpp"
#include "entities/subsystem.hpp"
#include "components/foodstuff.hpp"

#include <string>

namespace needs {

    struct Needs : ecs::ComponentCRTP<ecs::Component::Needs, Needs> {
        int food = 100;
        int max_food = 200;

        int sleep = 200;
        int max_sleep = 200;

        int happy = 200;
        int max_happy = 200;
    };

    struct NeedsSystem : ecs::SubSystem<NeedsSystem, Needs, ai::AI> {
        NeedsSystem() : ecs::SubSystem<NeedsSystem, Needs, ai::AI>(100) { }

        void update_item(ecs::Ent* e, Needs* nai, ai::AI* ai);

        static ecs::CRTPSystemFactory<NeedsSystem> factory;
    };

}