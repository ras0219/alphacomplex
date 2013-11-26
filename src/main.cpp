#include <algorithm>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>

#include "city.hpp"
#include "pathfind.hpp"
#include "graphics.hpp"
#include "tile.hpp"
#include "joblist.hpp"
#include "entity.hpp"
#include "room.hpp"
#include "log.hpp"

#include "viewstack.hpp"
#include "mainview.hpp"

#include <map>

using namespace std;
using namespace chrono;
/*************************/
const char* white = "#FFFFFF";

bool paused = false;

Graphics *gfx;

int main(int argc, char** argv) {
  srand(time(NULL));
  if (argc < 2)
    cin >> city;
  else
    fstream(argv[1]) >> city;

  Graphics g;
  gfx = &g;
  ViewStack vs;
  MainView mv(&vs);
  vs.push(&mv);

  g.c.push_back(&vs);

  while(!g.destroyed) {
    auto t = steady_clock::now();

    if (!paused) {
      for (auto e : AIEntity::ai_list)
        e->update();

      for (auto r : city.rooms)
        r->update();
    }

    g.repaint();

    g.handle_events(&vs);

    auto sleep_till = t + milliseconds(50);
    t = steady_clock::now();
    if (sleep_till > t) {
      auto ticks = duration_cast<microseconds>(sleep_till - t).count();
      usleep(ticks);
    }
  }
}
