#include "storage/storagesystem.hpp"
#include "storage/storagejob.hpp"
#include "entities/entity.hpp"
#include "components/room.hpp"
#include "components/item.hpp"
#include "components/foodstuff.hpp"
#include "job/job.hpp"
#include "job/joblist.hpp"
#include "city.hpp"

#include <algorithm>

namespace storage {

    StorageSystem::StorageSystem(City* c) : city(c), info_map(0, 0), timer(0) { assert(city); }

    void StorageSystem::update() {
        ++timer;

        if (timer == 1000) {
            timer = 0;
            delayed_update();
        }
    }

    void StorageSystem::delayed_update() {
        complete_jobs();
        build_info_map();
        issue_jobs();
        std::sort(jobs.begin(), jobs.end());
    }

    void StorageSystem::complete_jobs() {
        auto it = std::partition(jobs.begin(), jobs.end(),
                                 [](jobs_t::value_type& j) { return !j.second->completed(); });

        // Remove all the completed jobs.
        jobs.erase(it, jobs.end());
        // Maintain sorted.
        std::sort(jobs.begin(), jobs.end());
    }

    /// Creates (effectively) a bitmap of the storage situation
    ///@todo Use the job list to clear out any in-progress destinations
    void StorageSystem::build_info_map() {
        info_map.resize(city->getXSize(), city->getYSize());
        info_map.clear();

        for (auto e : ents) {
            auto storage = e->assert_get<Storage>();
            // For the moment, we only support storage rooms
            auto room = e->assert_get<Room>();

            for (auto y = room->r.y; y < room->r.y + room->r.h; ++y) {
                for (auto x = room->r.x; x < room->r.x + room->r.w; ++x) {
                    if (city->ent(x, y).empty()) {
                        info_map(x, y) = storage;
                    } else {
                        // Quick hack.
                        info_map(x, y) = this;
                    }
                }
            }
        }
    }

    void StorageSystem::issue_jobs() {
        auto storeloc = info_map.begin();

        for (auto it = global_set<item::Item>::begin();
             it != global_set<item::Item>::end();
             ++it)
        {
            // We only store items not currently being used.
            if ((*it)->locked) {
                continue;
            }

            // We only store food.
            if (!(*it)->parent->has<Foodstuff>()) {
                continue;
            }

            // Is this item currently in a stockpile?
            auto pt = (*it)->pos();
            if (info_map(pt.as_point()) != nullptr) {
                continue;
            }

            // Do we already have a job for this item?
            auto jptr = std::lower_bound(jobs.begin(), jobs.end(), *it,
                                         [](const jobs_t::value_type& l, item::Item* item) -> bool { return l.first < item; });
            if (jptr != jobs.end() && jptr->first == *it) {
                continue;
            }

            // Find a free space to store it

            for(;;) {
                if (storeloc == info_map.end()) {
                    return;
                }
                if (*storeloc != nullptr && *storeloc != this) {
                    break;
                }
                ++storeloc;
            }

            // We will only reach here if:
            // * the item is unlocked
            // * the item is food
            // * the item is not in a stockpile
            // * we do not have a job for the item
            // * we have found a free stockpile space
            point dest = info_map.pointof(storeloc);
            auto jb = make_storage_job({ city, dest.first, dest.second }, *it);
            job::JobList::getJoblist().add_job(jb);
            jobs.emplace_back(*it, jb);

            ++storeloc;
        }
    }

    void StorageSystem::insert(ecs::Ent* ent) {
        ents.insert(ent);
    }

    void StorageSystem::erase(ecs::Ent* ent) {
        ents.erase(ent);
    }

    ecs::CRTPSystemFactory<StorageSystem, true> StorageSystem::factory;

}
