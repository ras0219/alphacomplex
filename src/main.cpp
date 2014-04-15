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
#include "graphics.hpp"
#include "tile.hpp"
#include "joblist.hpp"
#include "log.hpp"
#include "clock.hpp"
#include "entities/system.hpp"
#include "views/viewstack.hpp"
#include "views/mainview.hpp"
#include "components/item.hpp"
#include "components/itemlock.hpp"

#include <map>


using namespace std;
using namespace chrono;
/*************************/
const char* white = "#FFFFFF";

unsigned long long graphics_frames, logic_frames;

bool paused = false;

Graphics *gfx;

std::vector<ecs::System*> systems;

int main(int, char **) {
  try {
    // creates a city of size 24 unit width and 30 unit height
    // this version also uses the current time a seed
    CityProperties cityP(24, 30);

    City city;

    systems = ecs::createSystems(&city);
    
    // generates a city using the city properties
    city.generate(cityP);

    srand((uint)time(NULL));

    gfx = new_graphics();
    ViewStack vs;
    MainView mv(&vs, &city);
    vs.push(&mv);

    gfx->c.push_back(&vs);

    auto last_gf = steady_clock::now();
    auto last_lf = steady_clock::now();
    auto last_rep = steady_clock::now();

    while (!gfx->destroyed) {
      auto t = steady_clock::now();

      if (t - last_lf >= microseconds(US_PER_LGC)) {
        if (!paused) {
          ++gametime;

          for (auto sys : systems)
            sys->update();

          item::ItemLock::finalize_deletes();
        }

        ++logic_frames;

        last_lf = t;
      }

      if (t - last_gf >= microseconds(US_PER_GFX)) {
        ++animtime;
        gfx->repaint();

        gfx->handle_events(&vs);

        ++graphics_frames;

        last_gf = t;
      }

      if (t - last_rep >= microseconds(US_PER_REP)) {
        stringstream ss;
        auto us = duration_cast<microseconds>(t - last_rep).count();
        ss << std::fixed << std::setprecision(1) << graphics_frames * 1000000.0 / us
          << " | " << logic_frames * 1000000.0 / us;

        graphics_frames = 0;
        logic_frames = 0;

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

  } catch(std::exception& e) {
    cerr << "An error occurred!" << endl;
    cerr << e.what() << endl;
    cin.get();
  }
  catch (...) {
    cerr << "An unknown error occurred!" << endl;
    cin.get();
  }
  return 0;
}
