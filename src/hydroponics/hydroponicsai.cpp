#include "hydroponics/hydroponicsai.hpp"
#include "hydroponics/hydroponics_table.hpp"
#include "hydroponics/hydroponicsjobs.hpp"
#include "components/room.hpp"
#include "components/jobprovider.hpp"
#include "utilities/assert_cast.hpp"

using namespace ai;
using namespace ecs;

namespace hydroponics {

    AI::timer_t HydroponicsAI::start(AI*) {
        return 1000;
    }

    void HydroponicsAI::update_tables(Room* r) {
        // Copy the furniture pointers into a vector
        std::vector<Furniture*> base_tables = r->find_furniture(hydroponics_table_properties);
        auto& tables = assert_cast<HydroponicsTable>(base_tables);

        // Update the table state listing
        std::sort(tables.begin(), tables.end());
        std::swap(tables, m_tables);
    }

    AI::timer_t HydroponicsAI::update(AI* ai) {
        Ent* room = ai->parent;
        auto jobprov = room->assert_get<job::JobProvider>();

        Room* r = room->assert_get<Room>();
        update_tables(r);

        if (m_tables.size() == 0) {
            return 1000;
        }

        // Add the jobs and update the growth stages
        for (auto table : m_tables) {
            switch (table->stage) {
            case HydroponicsTable::not_planted:
                jobprov->to_provide_jobs.emplace_back(make_plant_job(table));
                table->stage = HydroponicsTable::plant_requested;
                break;
            case HydroponicsTable::planted:
                if (rand() % 5 == 0) {
                    table->stage = HydroponicsTable::stage1;
                }
                break;
            case HydroponicsTable::stage1:
                if (rand() % 5 == 0) {
                    table->stage = HydroponicsTable::stage2;
                }
                break;
            case HydroponicsTable::stage2:
                if (rand() % 5 == 0) {
                    table->stage = HydroponicsTable::stage3;
                }
                break;
            case HydroponicsTable::stage3:
                if (rand() % 5 == 0) {
                    jobprov->to_provide_jobs.emplace_back(make_harvest_job(table));
                    table->stage = HydroponicsTable::ready;
                }
                break;
            default:
                break;
            }
        }
        return 1000;
    }

    std::string HydroponicsAI::desc = "Harvesting";

}
