#include "needs/seekfoodai.hpp"
#include "components/item.hpp"
#include "components/foodstuff.hpp"
#include "components/position.hpp"
#include "components/ai/pathai.hpp"
#include "needs/needs.hpp"

namespace needs {

    int SeekFoodAI::start(ai::AI* ai) {
        return update(ai);
    }
    int SeekFoodAI::update(ai::AI* ai) {
        if (!food) {
            return findfood(ai);
        }

        return findpath(ai);
    }

    const std::string& SeekFoodAI::description() const {
        return desc;
    }

    int SeekFoodAI::findfood(ai::AI* ai) {
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
    int SeekFoodAI::findpath(ai::AI* ai) {
        assert(food);

        auto dest = food->pos();
        if (dest != ai->parent->get<Position>()->as_point()) {
            // We are not at the food. We must travel!
            return ai->push_script(ai::make_pathai(point{ dest.x, dest.y }));
        }
        // We are at the food! Rejoice!
        return eatfood(ai);
    }

    int SeekFoodAI::eatfood(ai::AI* ai) {
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

    std::string SeekFoodAI::desc = "Seeking food";

    ai::AI::script_ptr make_seek_food_script() {
        return make_shared<SeekFoodAI>();
    }
}