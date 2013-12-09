#pragma once

#include "ai.hpp"
#include "pathfind.hpp"

#include <vector>

using std::vector;

struct PathAI : AIScript {
  PathAI(point d, int rate = 5) : walkrate(rate), dest(d) { }

  inline virtual int start(AI* ai) {
    Ent* c = ai->parent;
    //assert(c->city.tile(x2, y2).walkable());
    path = pathfind(c->city, c->x, c->y, dest.first, dest.second);
    pathp = path.rbegin();
    return walkrate;
  }

  inline virtual int update(AI* ai) {
    Ent* c = ai->parent;

    if (pathp != path.rend()) {
      assert(c->city.tile(pathp->first, pathp->second).walkable());
      c->set_loc(pathp->first, pathp->second);
      ++pathp;
    }

    if (pathp == path.rend())
      return complete(c);

    return walkrate;
  }

  int walkrate;
  point dest;
  vector<point> path;
  vector<point>::reverse_iterator pathp;
};

