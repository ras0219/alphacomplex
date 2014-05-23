#include "defs.hpp"
#include "needs/needssystem.hpp"
#include "needs/seekfoodai.hpp"

namespace needs {

    void NeedsSystem::update_item(ecs::Ent*, Needs* nai, ai::AI* ai) {
        if (nai->food > 0)
            --nai->food;

        if (nai->food <= nai->max_food / 5 && ai->current_priority() < 5) {
            announce("Red wizard needs food badly.");
            ai->add_task(make_seek_food_script(), 5);
        }
    }

    ecs::CRTPSystemFactory<NeedsSystem> NeedsSystem::factory;

}