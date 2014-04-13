#include "city.hpp"
#include "entities/garbage.hpp"
#include "windows.hpp"
#include "entities/citizen.hpp"
#include "components/renderable.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/pathai.hpp"
#include "components/ai/activityai.hpp"
#include "components/ai/callbackai.hpp"

#include <cstdio>

Ent* make_garbage(Point pos) {
  Ent* e = new Ent;
  e->add(new Position(pos));
  e->add(new Renderable(';'));
  return e;
}


std::shared_ptr<Job> make_garbage_job(Ent* e) {
  Position* pos = e->assert_get<Position>();

  clearance c = {
    (Security::Mask)(Security::INFRARED |
                     Security::RED),
    Department::FACILITIES
  };

  auto ais = std::make_shared<SequenceAI>();
  ais->add_task(std::make_shared<PathAI>(point{ pos->x(), pos->y() }));
  ais->add_task(std::make_shared<ActivityAI>(100));
  ais->add_task(make_callbackai([=](AI*){ delete e; }));

  return make_shared<Job>("Clean Garbage", c, std::move(ais));
}
