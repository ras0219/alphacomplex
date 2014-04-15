#include "storage/storageroom.hpp"
#include "storage/storage.hpp"
#include "storage/storagesystem.hpp"
#include "entities/citizen.hpp"
#include "components/ai/job.hpp"
#include "components/ai/callbackai.hpp"
#include "components/ai/sequenceai.hpp"
#include "components/ai/pathai.hpp"
#include "components/room.hpp"
#include "components/jobprovider.hpp"
#include "components/item.hpp"
#include "components/itemlock.hpp"
#include "components/foodstuff.hpp"
#include "city.hpp"
#include "windows.hpp"
#include "point.hpp"

#include <cstdlib>
#include <cstdio>

using namespace ai;
using namespace job;
using namespace item;

namespace storage {

    struct StorageRoomAI;

    std::shared_ptr<Job> make_storeitem_job(const Point& p, StorageRoomAI* srai);

    struct StorageRoomAI : AIScript {
        ///@bug Overschedules squares
        virtual AI::timer_t update(AI* ai) override {
            auto e = ai->parent;
            JobProvider* jobprov = e->assert_get<JobProvider>();

            Room* room = e->assert_get<Room>();
            const Rect& r = room->r;
            // Try to find an empty square in the rect
            for (int y = r.y; y < r.h + r.y; ++y) {
                for (int x = r.x; x < r.w + r.x; ++x) {
                    if (r.city->ent(x, y).size() > 0) {
                        // Filled...
                        continue;
                    }
                    // We found an empty square.
                    jobprov->to_provide_jobs.emplace_back(make_storeitem_job({ r.city, x, y }, this));
                    return 1000;
                }
            }

            return 1000;
        }

        virtual const std::string& description() const override {
            return desc;
        }
    private:
        static std::string desc;
    };

    std::string StorageRoomAI::desc = "Requesting items for storage";

    struct StoreItemAI : AIScript {
        StoreItemAI(const Point& p, StorageRoomAI* sr) : dest(p), storageroomai(sr) { assert(sr); }

        virtual AI::timer_t update(AI* ai) {
            // Have we located an item to transport?
            if (!item) {
                return find_item(ai);
            }
            // Do we have the item?
            if (item->container != ai->parent->assert_get<Item>()) {
                return get_item(ai);
            }
            // Are we at the destination?
            if (dest != ai->parent->assert_get<Position>()->as_point()) {
                return go_to_store(ai);
            }
            // Finally, place the item at the destination
            return store_item(ai);
        }

        virtual const std::string& description() const override {
            return desc;
        }

        AI::timer_t find_item(AI* ai) {
            assert(!item);
            auto it = global_set<Item>::begin();
            while (it != global_set<Item>::end()) {
                if ((*it)->locked) {
                    ++it;
                    continue;
                }

                // For now, we're going to work on storing food.
                if ((*it)->parent->has<Foodstuff>()) {
                    if ((item = (*it)->try_lock())) {
                        // Found an item.

                        ///@todo kick off another storage job in parallel
                        return update(ai);
                    }
                    // I was unable to lock the item.
                }
                ++it;
            }
            // Unable to locate an item. This is good enough.
            return ai->pop_script();
        }
        AI::timer_t get_item(AI* ai) {
            assert(item);
            auto item_pos = item->parent->assert_get<Item>()->pos();
            if (ai->parent->assert_get<Position>()->as_point() != item_pos) {
                // Travel to item location
                return ai->push_script(make_pathai(item_pos.as_point()));
            }
            // Move item into my inventory
            delete item->parent->remove<Position>();
            item->insert_into(ai->parent->assert_get<Item>());
            return update(ai);
        }
        AI::timer_t go_to_store(AI* ai) {
            assert(item);
            assert(ai->parent->assert_get<Position>()->as_point() != dest);

            // Travel to storage destination
            return ai->push_script(make_pathai(dest.as_point()));
        }
        AI::timer_t store_item(AI* ai) {
            assert(item);
            assert(!item->parent->has<Position>());
            assert(ai->parent->assert_get<Position>()->as_point() == dest);
            item->remove_from();
            item->parent->emplace<Position>(dest);

            // Completed task.
            ///@todo Spin off another task?
            return ai->pop_script();
        }

    private:
        Point dest;
        StorageRoomAI* storageroomai;
        ItemLock item;

        static std::string desc;
    };

    std::string StoreItemAI::desc = "Storing Item";

    ecs::Ent* make_storageroom(const Rect& rec) {
        ecs::Ent* r = new ecs::Ent;

        r->emplace<Room>(rec);
        //r->emplace<AI>(make_shared<StorageRoomAI>());
        //r->emplace<JobProvider>();

        //r->emplace_system<AISystem>();
        //r->emplace_system<JobProviderSystem>();
        r->emplace<Storage>();
        r->emplace_system<StorageSystem>();
        return r;
    }

    std::shared_ptr<Job> make_storeitem_job(const Point& p, StorageRoomAI* srai) {
        return make_shared<Job>(
            "Store Items",
            clearance{ Security::ALL, Department::FACILITIES },
            std::make_shared<StoreItemAI>(p, srai)
            );
    }

}