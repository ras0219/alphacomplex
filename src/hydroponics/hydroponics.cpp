#include "point.hpp"
#include "components/room.hpp"
#include "components/ai/ai.hpp"
#include "job/jobprovider.hpp"
#include "hydroponics/hydroponics.hpp"
#include "hydroponics/hydroponicsai.hpp"

#include <memory>

using namespace ecs;
using namespace job;
using namespace ai;

namespace hydroponics {

    Ent* make_hydroponics_room(const Rect& r) {
        Ent* e = new Ent;

        e->emplace<Room>(r);

        e->emplace<JobProvider>();
        e->emplace<AI>(std::make_shared<HydroponicsAI>());

        e->emplace_system<AISystem>();
        e->emplace_system<JobProviderSystem>();
        return e;
    }

}
