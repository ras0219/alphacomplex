#pragma once

#include "ai.hpp"
#include "pathfind.hpp"
#include "components/movable.hpp"

#include <vector>
#include <cassert>

struct PathAI : AIScript {
  PathAI(point d, AI::timer_t rate = 5) : walkrate(rate), dest(d), desc("Walking") { }

  inline virtual int start(AI* ai) {
    Ent* c = ai->parent;
    assert(c->has<Position>());
    Position* pos = c->get<Position>();

    path = pathfind(pos->city(), pos->x(), pos->y(),
                    dest.first, dest.second);
    pathp = path.rbegin();

    if (pathp == path.rend())
      return complete(ai);

    return walkrate;
  }

  inline virtual int update(AI* ai) {
    return update_impl(ai);
  }

  inline int update_impl(AI* ai) {
    Ent* c = ai->parent;
    assert(c->has<Movable>());
    Movable* mov = c->get<Movable>();

    if (pathp != path.rend()) {
      assert(mov->pos.city->tile(pathp->first, pathp->second).walkable());
      mov->set(*pathp);
      ++pathp;
    }

    if (pathp == path.rend())
      return complete(ai);

    return walkrate;
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