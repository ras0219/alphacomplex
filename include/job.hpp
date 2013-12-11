#pragma once

#include "defs.hpp"
#include "windows.hpp"
#include "ai.hpp"
#include "clearance.hpp"

#include <cassert>
#include <vector>

struct Job {
  Job(string d, Clearance c, AIScript* ais)
    : desc(d), clear(c), scr(ais), state(UNRESERVED) { }
  ~Job() { if (scr) delete scr; }

  inline int description(char* buf, size_t n) const {
    return snprintf(buf, n, "%s", desc.c_str());
  }
  inline Clearance clearance() const { return clear; }

  inline AIScript* script() {
    assert(scr != nullptr);
    AIScript* r = scr;
    scr = nullptr;
    return r;
  }

  inline bool available() const { return state == UNRESERVED; }
  inline bool unavailable() const { return state != UNRESERVED; }
  inline bool completed() const { return state == COMPLETED; }

  inline void reserve() { assert(state == UNRESERVED); state = RESERVED; }
  inline void complete() { assert(state == RESERVED); state = COMPLETED; }

  string desc;
  Clearance clear;
  AIScript* scr;

  enum State {
    UNRESERVED,
    RESERVED,
    COMPLETED
  } state;
};
