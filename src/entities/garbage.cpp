#include "city.hpp"
#include "entities/garbage.hpp"
#include "windows.hpp"
#include "job/joblist.hpp"
#include "job/job.hpp"
#include "entities/citizen.hpp"
#include "components/renderable.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/aidsl.hpp"
#include "components/ai/callbackai.hpp"
#include "components/clearance.hpp"
#include "components/position.hpp"
#include "components/item.hpp"

#include <cstdio>

item::ItemProperties garbage_properties = {
    "garbage",
    1
};

void spawn_garbage(const Point& pos) {
    auto g = make_garbage(pos);
    job::JobList::getJoblist().add_job(make_garbage_job(g));
}

ecs::Ent* make_garbage(const Point& pos) {
    auto e = new ecs::Ent;
    e->emplace<Position>(pos);
    e->emplace<Renderable>(';');
    e->emplace<item::Item>(garbage_properties);
    return e;
}

std::shared_ptr<job::Job> make_garbage_job(ecs::Ent* e) {
  Position* pos = e->assert_get<Position>();

  clearance c = {
    (Security::Mask)(Security::INFRARED |
                     Security::RED),
    Department::FACILITIES
  };

  auto ais = std::make_shared<ai::SequenceAI>();
  ais->add_task(ai::make_do_at(pos->as_point().as_point(), 100, "Cleaning garbage"));
  ais->add_task(ai::make_callbackai([=](ai::AI*){ delete e; }));

  return std::make_shared<job::Job>("Clean Garbage", c, std::move(ais));
}
