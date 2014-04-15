#pragma once

#include "components/ai/ai.hpp"
#include <vector>

struct Room;

namespace hydroponics {

    struct HydroponicsTable;

    struct HydroponicsAI : ai::AIScript {
        virtual timer_t start(ai::AI*) override;
        virtual timer_t update(ai::AI* ai) override;
        virtual const std::string& description() const override {
            return desc;
        }

    private:
        void update_tables(Room* r);

        std::vector<HydroponicsTable*> m_tables;
        static std::string desc;
    };

}