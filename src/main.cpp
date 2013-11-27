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

#ifndef _WIN32
#include <unistd.h>
#else
#include <thread>
#endif

#include "city.hpp"
#include "pathfind.hpp"
#include "graphics.hpp"
#include "tile.hpp"
#include "joblist.hpp"
#include "entity.hpp"
#include "room.hpp"
#include "log.hpp"
#include "clock.hpp"
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
  City city;

  srand(time(NULL));
  if (argc < 2)
    cin >> city;
  else
    fstream(argv[1]) >> city;

  Graphics g;
  gfx = &g;
  ViewStack vs;
  MainView mv(&vs, city);
  vs.push(&mv);

  g.c.push_back(&vs);

  #define MS_PER_GFX 33
  #define MS_PER_LGC 1

  auto last_gf = steady_clock::now();
  auto last_lf = steady_clock::now();

  while(!g.destroyed) {
    auto t = steady_clock::now();

    if (t - last_lf >= milliseconds(MS_PER_LGC)) {
      if (!paused) {
        ++gametime;
        for (auto e : AIEntity::ai_list)
          e->update();

        for (auto r : city.rooms)
          r->update();

      }
      last_lf = t;
    }

    if (t - last_gf >= milliseconds(MS_PER_GFX)) {
      ++animtime;
      g.repaint();

      g.handle_events(&vs);
    }

    auto sleep_till = last_lf + milliseconds(MS_PER_LGC);
    if (sleep_till < last_gf + milliseconds(MS_PER_GFX))
      sleep_till = last_gf + milliseconds(MS_PER_GFX);

    t = steady_clock::now();
    if (sleep_till > t) {
      #ifndef _WIN32
      auto ticks = duration_cast<microseconds>(sleep_till - t).count();
      usleep(ticks);
      #else
      this_thread::sleep_until(sleep_till);
      #endif
    }
  }

  return 0;
}
