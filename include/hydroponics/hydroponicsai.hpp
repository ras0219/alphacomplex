#pragma once

#include "components/ai/ai.hpp"
#include <vector>

struct Room;

struct HydroponicsAI : AIScript {
    virtual AI::timer_t start(AI*) override;
    virtual AI::timer_t update(AI* ai) override;
    virtual const std::string& description() const override {
        return desc;
    }

private:
    void update_tables(Room* r);

    std::vector<struct HydroponicsTable*> m_tables;
    static std::string desc;
};