#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>

#include "city.hpp"
#include "pathfind.hpp"
#include "tile.hpp"

using namespace std;

/*************************/

const char* white = "#FFFFFF";

int main() {
  cin >> city;
  cout << "Created city.\n";

  Dwarf d(9,9);
  d.insert_after(city.ent(9,9));

  Elf d2(5,5);
  d2.insert_after(city.ent(5,5));

  Graphics g;
  g.clear();

  for (int x=0;x<5;++x) {
    cout << "----------- Step " << x << '\n';
    city.render(g);

    g.print();

    Entity* e = Entity::GLOB_ENTLIST;
    while (e != nullptr) {
      e->update();
      e = e->g_next;
    }
  }

  g.c = &city;

  while(!g.destroyed) {
    auto t = steady_clock::now();

    Entity* e = Entity::GLOB_ENTLIST;
    while (e != nullptr) {
      e->update();
      e = e->g_next;
    }

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
