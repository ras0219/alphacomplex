#include <algorithm>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>
#include <string>
#include <X11/keysym.h>
#include <unistd.h>

#include "component.hpp"
#include "city.hpp"
#include "pathfind.hpp"
#include "graphics.hpp"
#include "tile.hpp"
#include "joblist.hpp"
#include "entity.hpp"
#include "room.hpp"
#include "hud.hpp"
#include "input.hpp"
#include "mapview.hpp"

#include <map>

using namespace std;
using namespace chrono;
/*************************/
const char* white = "#FFFFFF";

struct MyController : Controller {
  MyController(Graphics& g_) : g(g_) { }

  virtual void handle_keypress(KeySym ks) {
    if (viewmap.find(ks) != viewmap.end()) {
      g.c.back() = viewmap[ks];
      return;
    }

    switch (ks) {
    case XK_Escape:
      g.c.back() = main;
      return;
    default:
      g.destroy();
      return;
    }
  }

  Graphics& g;

  Component* main;
  map<KeySym, Component*> viewmap;
};

struct ComponentList : Component {
  virtual void render(Graphics& g) {
    for (auto c : comps)
      c->render(g);
  }

  ComponentList() {}
  ComponentList(const initializer_list<Component*>& il) : comps(il) { }

  vector<Component*> comps;
};

int main() {
  srand(time(NULL));
  cin >> city;
  cout << "Created city.\n";

  Graphics g;

  MyController mc(g);

  MapView mv(city.getXSize(), city.getYSize(), &city);

  Hud imeter;

  JobListing activelist(20, &active_jobs, "Active Jobs");
  JobListing pendinglist(80, &jobs, "Pending Jobs");

  ComponentList clist = { &mv, &activelist, &pendinglist, &imeter };

  UnitListing ulist;
  ComponentList uscreen = { &imeter, &ulist };

  mc.main = &clist;
  mc.viewmap[XK_u] = &ulist;

  g.c.push_back(&clist);

  cout << city.ent(4,4) << " " << city.ent(4,4)->next << endl;

  while(!g.destroyed) {
    auto t = steady_clock::now();

    for (auto e : AIEntity::ai_list)
      e->update();

    for (auto r : city.rooms)
      r->update();

    g.repaint();

    g.handle_events(&mc);

    auto sleep_till = t + milliseconds(50);
    t = steady_clock::now();
    if (sleep_till > t) {
      auto ticks = duration_cast<microseconds>(sleep_till - t).count();
      usleep(ticks);
    }
  }
}
