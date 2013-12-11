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
#include "ai.hpp"
#include "movable.hpp"

#include <map>


using namespace std;
using namespace chrono;
/*************************/
const char* white = "#FFFFFF";

microseconds us_per_gf, us_per_lf;

bool paused = false;

Graphics *gfx;

vector<System*> systems = { &aisystem, &smsystem };

int main(int argc, char** argv) {
  City city;

  srand(time(NULL));
  if (argc < 2)
    cin >> city;
  else
    fstream(argv[1]) >> city;

  gfx = new_graphics();
  ViewStack vs;
  MainView mv(&vs, city);
  vs.push(&mv);

  gfx->c.push_back(&vs);

  auto last_gf = steady_clock::now();
  auto last_lf = steady_clock::now();
  auto last_rep = steady_clock::now();

  while(!gfx->destroyed) {
    auto t = steady_clock::now();

    if (t - last_lf >= microseconds(US_PER_LGC)) {
      if (!paused) {
        ++gametime;
        for (auto sys : systems)
          sys->update();
        
        for (auto r : city.rooms)
          r->update();

      }
      us_per_lf = duration_cast<microseconds>(t - last_lf);
      last_lf = t;
    }

    if (t - last_gf >= microseconds(US_PER_GFX)) {
      ++animtime;
      gfx->repaint();

      gfx->handle_events(&vs);
      us_per_gf = duration_cast<microseconds>(t - last_gf);
      last_gf = t;
    }

    if (t - last_rep >= microseconds(US_PER_REP)) {
      stringstream ss;
      ss << 1000000 / us_per_gf.count() << " | " << 1000000 /  us_per_lf.count() << ".";
      announce(ss.str());
      last_rep = t;
    }

    auto sleep_till = last_lf + microseconds(US_PER_LGC);
    if (sleep_till > last_gf + microseconds(US_PER_GFX))
      sleep_till = last_gf + microseconds(US_PER_GFX);

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
