#include "entities/filestorage.hpp"
#include "entities/entity.hpp"
#include "components/position.hpp"
#include "components/renderable.hpp"
#include "components/room.hpp"
#include "components/furniture.hpp"
#include "components/ai/ai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/pathai.hpp"
#include "components/ai/callbackai.hpp"
#include "components/jobprovider.hpp"
#include "components/item.hpp"

#include <algorithm>

extern int influence;

ItemProperties filingcabinet_properties = {
  "Filing Cabinet",
  150
};

Ent* make_filingcabinet(const Point& p) {
  Ent* e = new Ent;
  e->emplace<Renderable>('n');
  e->emplace<Furniture>(p);
  e->emplace<Item>(filingcabinet_properties);
  return e;
}

std::shared_ptr<Job> make_filing_job(int x1, int y1, int x2, int y2, Ent*) {
  std::shared_ptr<SequenceAI> script = std::make_shared<SequenceAI>();

  script->add_task(std::make_shared<PathAI>(point(x1, y1)));
  script->add_task(std::make_shared<PathAI>(point(x2, y2)));
  script->add_task(make_callbackai([=](AI*) { ++influence; }));

  return make_shared<Job>(
    "Reorganize filing system",
    clearance{ Security::ALL, Department::ALL },
    script
    );
}

struct FilestorageAI : AIScript {
  virtual int start(AI*) {
    return 1;
  }
  virtual int update(AI* ai) {
    Ent* room = ai->parent;
    JobProvider* jobprov = room->assert_get<JobProvider>();

    for (uint x = 0; x < jobprov->provided_jobs.size(); ++x) {
      if (jobprov->provided_jobs[x]->completed())
        jobprov->provided_jobs.erase(jobprov->provided_jobs.begin() + x--);
    }

    if (jobprov->provided_jobs.size() > 0)
      // Do not create more jobs if we aren't done with the last reshuffling
      return 1000;

    if (rand() % 5 > 0)
      return 1;

    Room* r = room->assert_get<Room>();
    // Copy the furniture poiners into a vector
    std::vector<Furniture*> cabinets = r->find_furniture(filingcabinet_properties);
    // Are there enough cabinets to shuffle?
    if (cabinets.size() < 2) {
      // Not enough filing cabinets to reshuffle
      return 1000;
    }
    // Shuffle the vector
    std::random_shuffle(cabinets.begin(), cabinets.end());
    // How many jobs will we perform? Half the cabinets + 1 sounds good.
    size_t num_to_select = cabinets.size() / 2 + 1;

    // Add the jobs
    for (size_t n = 0; n < num_to_select - 1; ++n) {
      auto f1 = cabinets[n];
      auto f2 = cabinets[n + 1];
      jobprov->to_provide_jobs.emplace_back(make_filing_job(f1->x(), f2->y(), f2->x(), f2->y(), room));
    }
    // Generate the last job
    auto f1 = cabinets[num_to_select - 1];
    auto f2 = cabinets[0];
    jobprov->to_provide_jobs.emplace_back(make_filing_job(f1->x(), f2->y(), f2->x(), f2->y(), room));

    return 1;
  }
};

Ent* make_filestorage(const Rect& r) {
  Ent* e = new Ent;
  e->emplace<Room>(r);
  e->emplace<AI>(std::make_shared<FilestorageAI>());
  e->emplace<JobProvider>();

  e->add(&aisystem);
  e->add(&jpsystem);
  return e;
}