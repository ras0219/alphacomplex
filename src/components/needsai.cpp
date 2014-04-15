#include "components/ai/needsai.hpp"
#include "components/ai/activityai.hpp"
#include "components/ai/callbackai.hpp"
#include "utilities/global_set.hpp"
#include "components/item.hpp"
#include "components/itemlock.hpp"
#include "components/foodstuff.hpp"
#include "components/position.hpp"
#include "components/ai/pathai.hpp"

namespace needs {

    struct SeekFoodAI : ai::AIScript {
        virtual int start(ai::AI* ai) override {
            return update(ai);
        }
        virtual int update(ai::AI* ai) override {
            if (!food) {
                return findfood(ai);
            }

            return findpath(ai);
        }

        virtual const std::string& description() const override {
            return desc;
        }

        int findfood(ai::AI* ai) {
            auto it = global_set<item::Item>::begin();
            while (it != global_set<item::Item>::end()) {
                if ((*it)->locked) {
                    ++it;
                    continue;
                }

                if ((*it)->parent->has<Foodstuff>()) {
                    // I have found food!
                    if ((food = (*it)->try_lock())) {
                        // It's mine!
                        return findpath(ai);
                    }
                    // I was unable to lock the food.
                }
                ++it;
            }
            // Didn't find food.... :(
            // Try again later.
            return 200;
        }
        int findpath(ai::AI* ai) {
            assert(food);

            auto dest = food->pos();
            if (dest != ai->parent->get<Position>()->as_point()) {
                // We are not at the food. We must travel!
                return ai->push_script(ai::make_pathai(point{ dest.x, dest.y }));
            }
            // We are at the food! Rejoice!
            return eatfood(ai);
        }

        int eatfood(ai::AI* ai) {
            auto fs = food->parent->assert_get<Foodstuff>();
            auto needs = ai->parent->assert_get<Needs>();
            // Consume the food.
            needs->food += fs->amount;
            if (needs->food > needs->max_food)
                needs->food = needs->max_food;
            // Release and delete the food. It has been consumed.
            food.delete_reset();
            return ai->pop_script();
        }

        item::ItemLock food;
        static std::string desc;
    };


    std::string SeekFoodAI::desc = "Seeking food";

    ai::AI::script_ptr make_seek_food_script() {
        return make_shared<SeekFoodAI>();
    }

    void NeedsSystem::update_item(ecs::Ent*, Needs* nai, ai::AI* ai) {
        if (nai->food > 0)
            --nai->food;

        if (nai->food <= nai->max_food / 5 && ai->current_priority() < 5) {
            ai->add_task(make_seek_food_script(), 5);
        }
    }

    ecs::CRTPSystemFactory<NeedsSystem> NeedsSystem::factory;

}