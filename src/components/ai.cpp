#include "components/ai/ai.hpp"

bool AI::add_task(AI::script_ptr ais, AI::priority_t prior) {
    if (prior <= current_priority())
        return false;
    current_script()->suspend(this);
    {
        task_stack t;
        t.emplace_back(std::move(ais));
        tasks.emplace_back(prior, std::move(t));
    }
    timer = current_script()->start(this);
    return true;
}

AISystem SubSystem<AISystem, AI>::g_singleton;
