#pragma once

#include "ai.hpp"
#include "pathfind.hpp"
#include "components/movable.hpp"

#include <vector>
#include <cassert>

struct PathAI : AIScript {
  PathAI(point d, AI::timer_t rate = 5) : walkrate(rate), dest(d), desc("Walking") { }

  inline virtual AI::timer_t start(AI* ai) {
    Ent* c = ai->parent;
    Position* pos = c->assert_get<Position>();

    path = pathfind(pos->city(), pos->x(), pos->y(),
                    dest.first, dest.second);
    pathp = path.rbegin();

    if (pathp == path.rend()) {
        return check_end(ai, pos);
    } else {
        return walkrate;
    }
  }

  inline AI::timer_t check_end(AI* ai, Position* pos) {
        // Either we are at the destination, or we can't find a path.
        if (pos->x() == dest.first && pos->y() == dest.second) {
            // The journey is complete
            return ai->pop_script();
        }
        // Otherwise...... failure.
        return ai->fail_script();
  }

  inline virtual int update(AI* ai) {
    Ent* c = ai->parent;

    if (pathp != path.rend()) {
        Movable* mov = c->assert_get<Movable>();
        assert(mov->pos.city->tile(pathp->first, pathp->second).walkable());
        mov->set(*pathp);
        ++pathp;

        return walkrate;
    } else {
        return check_end(ai, c->assert_get<Position>());
    }
  }

  virtual const std::string& description() const override {
      return desc;
  }

private:
    AI::timer_t walkrate;
    point dest;
    vector<point> path;
    vector<point>::reverse_iterator pathp;
    std::string desc;
};

inline std::shared_ptr<PathAI> make_pathai(point p, int rate = 5) {
  return make_shared<PathAI>(p, rate);
}