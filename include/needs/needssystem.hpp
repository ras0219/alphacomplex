#pragma once

#include "needs.hpp"
#include "entities/subsystem.hpp"
#include "components/ai/ai.hpp"

namespace needs {

    struct NeedsSystem : ecs::SubSystem < NeedsSystem, Needs, ai::AI > {
        NeedsSystem() : ecs::SubSystem<NeedsSystem, Needs, ai::AI>(100) { }

        void update_item(ecs::Ent* e, Needs* nai, ai::AI* ai);

        static ecs::CRTPSystemFactory<NeedsSystem> factory;
    };

}