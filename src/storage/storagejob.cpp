#include "storage/storagejob.hpp"

#include "point.hpp"
#include "components/position.hpp"
#include "job/job.hpp"

#include "components/ai/lockai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/callbackai.hpp"
#include "components/ai/aidsl.hpp"
#include "components/position.hpp"

std::shared_ptr<job::Job> make_storage_job(const Point& dest, item::Item* item) {
    auto e = item->parent;
    auto pos = e->assert_get<Position>();
    clearance c = {
        (Security::Mask)(Security::INFRARED | Security::RED),
        Department::FACILITIES
    };

    auto ais = std::make_shared<ai::SequenceAI>();
    ais->add_task(ai::make_do_at(pos->as_point().as_point(), 10, "Collecting Item"));
    ais->add_task(ai::make_callbackai([=](ai::AI* ai){
        delete e->remove<Position>();
        auto it = ai->parent->assert_get<item::Item>();
        auto it2 = e->assert_get<item::Item>();
        it2->insert_into(it);
    }));
    ais->add_task(ai::make_do_at(dest.as_point(), 10, "Storing Item"));
    ais->add_task(ai::make_callbackai([=](ai::AI*){
        auto it2 = e->assert_get<item::Item>();
        it2->remove_from();
        e->emplace<Position>(dest);
    }));

    auto script = ai::make_lockai(e->assert_get<item::Item>(), ais);
    return std::make_shared<job::Job>("Store Food", c, script);
}
