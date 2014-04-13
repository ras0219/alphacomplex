#include "components/ai/ai.hpp"
#include "views/hud.hpp"

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

// These are just some inline method calls, no worries
AI::timer_t AI::pop_script() {
    assert(tasks.size() > 0);
    assert(current_task().size() > 0);
    current_task().pop_back();
    if (current_task().size() == 0) {
        // Resume the previous task
        assert(tasks.size() > 1);
        tasks.pop_back();
        return current_script()->resume(this);
    }
    return current_script()->update(this);
}

AI::timer_t AI::fail_script() {
    assert(tasks.size() > 0);
    assert(current_task().size() > 0);
    if (current_task().size() == 1) {
        // Resume the previous task.... but this really shouldn't happen.
        A11s::instance.announce(std::string("A task failed: ") + current_script()->description());
        current_task().pop_back();

        assert(tasks.size() > 1);
        tasks.pop_back();
        return current_script()->resume(this);
    }
    // Transmit the failure to the parent
    current_task().pop_back();
    return current_script()->failure(this);
}

AI::timer_t AI::push_script(AI::script_ptr ais) {
    current_task().emplace_back(std::move(ais));
    return current_script()->start(this);
}

AISystem SubSystem<AISystem, AI>::g_singleton;
