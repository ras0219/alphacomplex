#include "point.hpp"
#include "components/room.hpp"
#include "components/ai/ai.hpp"
#include "components/jobprovider.hpp"
#include "hydroponics/hydroponics.hpp"
#include "hydroponics/hydroponicsai.hpp"

#include <memory>

Ent* make_hydroponics_room(const Rect& r) {
    Ent* e = new Ent;

    e->emplace<Room>(r);

    e->emplace<JobProvider>();
    e->emplace<AI>(std::make_shared<HydroponicsAI>());

    e->add(&AISystem::singleton());
    e->add(&JobProviderSystem::singleton());
    return e;
}
