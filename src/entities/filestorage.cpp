#include "entities/filestorage.hpp"
#include "entities/entity.hpp"
#include "components/position.hpp"
#include "components/renderable.hpp"
#include "components/room.hpp"
#include "components/furniture.hpp"
#include "components/ai/ai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/callbackai.hpp"
#include "components/item.hpp"
#include "components/ai/aidsl.hpp"
#include "job/jobprovider.hpp"

#include <algorithm>

using namespace ecs;
using namespace job;
using namespace item;
using namespace ai;

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
  auto script = std::make_shared<SequenceAI>();

  script->add_task(make_do_at(point(x1, y1), 15, "Retrieve document"));
  script->add_task(make_do_at(point(x2, y2), 15, "File document"));
  script->add_task(make_callbackai([=](AI*) { ++influence; }));

  return std::make_shared<Job>(
    "Reorganize filing system",
    clearance{ Security::ALL, Department::ALL },
    script
    );
}

struct FilestorageAI : AIScript {
    virtual AI::timer_t start(AI*) override {
        return 1;
    }
    virtual AI::timer_t update(AI* ai) override {
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

    virtual const std::string& description() const override {
        return desc;
    }

private:
    static std::string desc;
};

std::string FilestorageAI::desc = "Storing files";

Ent* make_filestorage(const Rect& r) {
  Ent* e = new Ent;
  e->emplace<Room>(r);
  e->emplace<AI>(std::make_shared<FilestorageAI>());
  e->emplace<JobProvider>();

  e->add(AISystem::factory.singleton());
  e->add(JobProviderSystem::factory.singleton());
  return e;
}