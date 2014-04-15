#include "components/ai/aidsl.hpp"
#include "components/position.hpp"
#include "components/ai/pathai.hpp"

namespace ai {
    struct DoAtAI : AIScript {
    private:
        enum class State {
            travel,
            activity
        };

    public:
        DoAtAI(point p, AI::timer_t d, AI::timer_t w, std::string s)
            : state(State::travel),
            dest(p),
            duration(d),
            walkrate(w),
            desc(std::move(s)) {}

        virtual AI::timer_t start(AI* ai) override {
            return update(ai);
        }

        virtual void suspend(AI*) override {
            // Lose all progress towards the activity goal
            state = State::travel;
        }

        virtual AI::timer_t update(AI* ai) override {
            auto c = ai->parent;
            auto pos = c->assert_get<Position>();
            if (pos->as_point().as_point() != dest) {
                state = State::travel;
                return ai->push_script(make_pathai(dest, walkrate));
            } else {
                if (state == State::travel) {
                    state = State::activity;
                    return duration;
                } else if (state == State::activity) {
                    return ai->pop_script();
                } else {
                    assert(false);
                    return 0;
                }
            }
        }

        virtual const std::string& description() const override {
            return desc;
        }

    private:
        State state;
        point dest;
        timer_t duration;
        timer_t walkrate;
        std::string desc;
    };

    AI::script_ptr make_do_at(point p, int duration, std::string description, int walkrate) {
        return make_shared<DoAtAI>(p, duration, walkrate, description);
    }
}