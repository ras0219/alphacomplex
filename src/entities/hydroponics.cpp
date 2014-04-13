#include "entities/hydroponics.hpp"
#include "components/room.hpp"
#include "components/ai/ai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/pathai.hpp"
#include "components/ai/activityai.hpp"
#include "components/ai/callbackai.hpp"
#include "components/ai/lockai.hpp"
#include "components/jobprovider.hpp"
#include "components/item.hpp"
#include "components/furniture.hpp"
#include "components/renderable.hpp"
#include "entities/entity.hpp"

#include "entities/foods.hpp"

#include <memory>

ItemProperties hydroponics_table_properties = {
  "Hydroponics Table",
  100
};

Ent* make_hydroponics_table(Point const& p) {
  Ent* e = new Ent;
  e->emplace<Renderable>('T');
  e->emplace<Furniture>(p);
  e->emplace<Item>(hydroponics_table_properties);
  return e;
}

std::shared_ptr<Job> make_harvest_job(Furniture* table) {
  std::shared_ptr<SequenceAI> script = std::make_shared<SequenceAI>();
  script->add_task(make_pathai(point(table->x(), table->y())));
  script->add_task(make_activityai(200));
  script->add_task(make_callbackai([=](AI*) {
    Point p = table->as_point();
    p.city->add_ent(p.x, p.y, make_lettuce(p));
  }));

  return make_shared<Job>(
    "Harvest plants",
    clearance{ Security::ALL, Department::AGRICULTURE },
    make_lockai(table->parent->assert_get<Item>(), script)
    );
}

struct HydroponicsAI : AIScript {
  virtual int start(AI*) {
    return 1000;
  }
  virtual int update(AI* ai) {
    Ent* room = ai->parent;
    JobProvider* jobprov = room->assert_get<JobProvider>();

    for (uint x = 0; x < jobprov->provided_jobs.size(); ++x) {
      if (jobprov->provided_jobs[x]->completed())
        jobprov->provided_jobs.erase(jobprov->provided_jobs.begin() + x--);
    }

    if (jobprov->provided_jobs.size() > 4)
      // Do not create more jobs if there are still plants to be harvested.
      return 1000;

    if (rand() % 5 > 0)
      return 1000;

    Room* r = room->assert_get<Room>();
    // Copy the furniture poiners into a vector
    std::vector<Furniture*> tables = r->find_furniture(hydroponics_table_properties);
    // Return if there are no hydroponics tables
    if (tables.size() == 0) {
      return 1000;
    }
    // Shuffle the vector
    std::random_shuffle(tables.begin(), tables.end());
    // How many jobs will we perform? Half the tables + 1 sounds good.
    size_t num_to_select = tables.size() / 2 + 1;

    // Add the jobs
    for (size_t n = 0; n < num_to_select; ++n) {
      auto f = tables[n];
      jobprov->to_provide_jobs.emplace_back(make_harvest_job(f));
    }

    return 1000;
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
    static std::string desc;
};

std::string HydroponicsAI::desc = "Harvesting";

Ent* make_hydroponics_room(const Rect& r) {
  Ent* e = new Ent;

  e->emplace<Room>(r);

  e->emplace<AI>(std::make_shared<HydroponicsAI>());
  e->emplace<JobProvider>();

  e->add(&AISystem::singleton());
  e->add(&jpsystem);
  return e;
}
