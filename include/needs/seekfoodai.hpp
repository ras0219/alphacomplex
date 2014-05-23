#pragma once

#include "components/ai/ai.hpp"
#include "components/itemlock.hpp"

namespace needs {

    struct SeekFoodAI : ai::AIScript {
        virtual int start(ai::AI* ai) override;
        virtual int update(ai::AI* ai) override;

        virtual const std::string& description() const override;

    private:
        int findfood(ai::AI* ai);
        int findpath(ai::AI* ai);

        int eatfood(ai::AI* ai);

        item::ItemLock food;
        static std::string desc;
    };

    ai::AI::script_ptr make_seek_food_script();
}
