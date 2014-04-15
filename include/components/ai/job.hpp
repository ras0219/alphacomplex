#pragma once

#include "defs.hpp"
#include "windows.hpp"
#include "ai.hpp"
#include "components/clearance.hpp"

#include <cassert>
#include <vector>
#include <string>

namespace job {

    struct Job {
        Job(std::string d, clearance c, ai::AI::script_ptr ais) : desc(d), clear(c), scr(ais), state(UNRESERVED) { }

        inline const std::string& description() const {
            return desc;
        }
        inline struct clearance clearance() const { return clear; }

        inline ai::AI::script_ptr script() {
            return scr;
        }

        inline bool available() const { return state == UNRESERVED; }
        inline bool unavailable() const { return state != UNRESERVED; }
        inline bool completed() const { return state == COMPLETED; }

        inline void reserve() { assert(state == UNRESERVED); state = RESERVED; }
        inline void complete() { assert(state == RESERVED); state = COMPLETED; }

        std::string desc;
        struct clearance clear;
        ai::AI::script_ptr scr;

        enum State {
            UNRESERVED,
            RESERVED,
            COMPLETED
        } state;
    };

}
