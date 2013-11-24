#include <algorithm>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>
#include <string>

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

using namespace std;
using namespace chrono;
/*************************/
const char* white = "#FFFFFF";

int main() {
  srand(time(NULL));
  cin >> city;
  cout << "Created city.\n";

  Graphics g(city.getXSize(), city.getYSize());
  g.clear();

  Hud imeter;

  g.c.push_back(&city);
  g.c.push_back(new JobListing(20, &active_jobs, "Active Jobs"));
  g.c.push_back(new JobListing(80, &jobs, "Pending Jobs"));
  g.c.push_back(&imeter);

  cout << city.ent(4,4) << " " << city.ent(4,4)->next << endl;

  while(!g.destroyed) {
    auto t = steady_clock::now();

    Entity* e = Entity::GLOB_ENTLIST;
    while (e != nullptr) {
      e->update();
      e = e->g_next;
    }

    for (auto r : city.rooms)
      r->update();

    g.repaint();

    g.handle_events();

    auto sleep_till = t + milliseconds(50);
    t = steady_clock::now();
    if (sleep_till > t) {
      auto ticks = duration_cast<microseconds>(sleep_till - t).count();
      usleep(ticks);
    }
  }

  // vector<point> path = pathfind(city, 0, 0, 10, 10);

  // for (auto p : path)
  //   cout << p.first << ", " << p.second << endl;
}
