#include "components/ai/sequenceai.hpp"

AI::timer_t SequenceAI::start(AI* ai) {
    return update(ai);
}

AI::timer_t SequenceAI::update(AI* ai) {
    if (i == subs.size())
        return ai->pop_script();
    else
        return ai->push_script(subs[i++]);
}
