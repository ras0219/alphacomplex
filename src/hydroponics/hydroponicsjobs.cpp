#include "hydroponics/hydroponicsjobs.hpp"
#include "hydroponics/hydroponics_table.hpp"
#include "components/furniture.hpp"
#include "components/jobprovider.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/aidsl.hpp"
#include "components/ai/callbackai.hpp"
#include "components/ai/lockai.hpp"
#include "point.hpp"
#include "city.hpp"
#include "entities/foods.hpp"
#include "utilities/assert_cast.hpp"

std::shared_ptr<Job> make_harvest_job(Furniture* table) {
    auto script = std::make_shared<SequenceAI>();
    script->add_task(make_do_at(table->as_point().as_point(), 200, "Harvesting Plants"));
    script->add_task(make_callbackai([=](AI*) {
        assert_cast<HydroponicsTable>(table)->stage = HydroponicsTable::not_planted;
        Point p = table->as_point();
        p.city->add_ent(p.x, p.y, make_lettuce(p));
    }));

    return make_shared<Job>(
        "Harvest plants",
        clearance{ Security::ALL, Department::AGRICULTURE },
        make_lockai(table->parent->assert_get<Item>(), script)
        );
}

std::shared_ptr<Job> make_plant_job(Furniture* table) {
    auto script = std::make_shared<SequenceAI>();
    point seeds_loc = table->as_point().city->random_point();
    script->add_task(make_do_at(seeds_loc, 200, "Scrounging Seeds"));
    script->add_task(make_do_at(table->as_point().as_point(), 200, "Planting Seeds"));
    script->add_task(make_callbackai([=](AI*) {
        assert_cast<HydroponicsTable>(table)->stage = HydroponicsTable::planted;
    }));

    return make_shared<Job>(
        "Plant plants",
        clearance{ Security::ALL, Department::AGRICULTURE },
        make_lockai(table->parent->assert_get<Item>(), script)
        );
}
