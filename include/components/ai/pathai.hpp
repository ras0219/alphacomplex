#pragma once

#include "ai.hpp"
#include "pathfind.hpp"
#include "components/movable.hpp"

#include <vector>
#include <cassert>

struct PathAI : AIScript {
  PathAI(point d, int rate = 5) : walkrate(rate), dest(d) { }

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

  int walkrate;
  point dest;
  vector<point> path;
  vector<point>::reverse_iterator pathp;
};

inline std::shared_ptr<PathAI> make_pathai(point p, int rate = 5) {
  return make_shared<PathAI>(p, rate);
}