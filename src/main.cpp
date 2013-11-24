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
#include "elf.hpp"
#include "room.hpp"

using namespace std;
using namespace chrono;
/*************************/
int influence = 0;

struct Influenceometer : Component {
  virtual void render(Graphics& g) {
    stringstream out;
    out << "Influence: " << influence;
    g.drawString(5, 200, out.str(), Graphics::DEFAULT);
  }
};

const char* white = "#FFFFFF";

int main() {
  srand(time(NULL));
  cin >> city;
  cout << "Created city.\n";

  // Dwarf d(1,1);
  // d.insert_after(city.ent(1,1));

  // Elf e1(1,1), e2(1,1);
  // e1.insert_after(city.ent(1,1));
  // e2.insert_after(city.ent(1,1));

  Graphics g(city.getXSize(), city.getYSize());
  g.clear();

  Influenceometer imeter;

  g.c.push_back(&city);
  g.c.push_back(new JobListing(200, 20, &active_jobs, "Active Jobs"));
  g.c.push_back(new JobListing(200, 80, &jobs, "Pending Jobs"));
  g.c.push_back(&imeter);

  cout << city.ent(4,4) << " " << city.ent(4,4)->next << endl;

  while(!g.destroyed) {
    auto t = steady_clock::now();

    Entity* e = Entity::GLOB_ENTLIST;
    while (e != nullptr) {
      e->update();
      cout << e->rawname() << "\n";
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
