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

bool paused = false;

Graphics *gfx;

struct ComponentList : Component {
  virtual void render(Graphics& g) {
    for (auto c : comps)
      c->render(g);
  }

  ComponentList() {}
  ComponentList(const initializer_list<Component*>& il) : comps(il) { }

  vector<Component*> comps;
};

struct ViewStack : Controller, Component {
  ViewStack(City* c)
    : mv(c->getXSize(), c->getYSize(), c),
      activelist(20, &active_jobs, "Active Jobs"),
      pendinglist(80, &jobs, "Pending Jobs"),
      mainpage({ &mv, &activelist, &pendinglist, &hud }),
      unitpage({ &hud, &unitlist }),
      cur_page(&mainpage)
    { }
  ViewStack(const ViewStack&) = delete;
  ViewStack& operator=(const ViewStack&) = delete;

  virtual void render(Graphics& g) {
    cur_page->render(g);
  }

  virtual void handle_keypress(KeySym ks) {
    if (cur_page == &mainpage) {
      switch (ks) {
      case XK_u:
      case XK_U:
        cur_page = &unitpage;
        break;
      case XK_r:
        if (influence > 15) {
          influence -= 15;
          Citizen* e = new Citizen(1,1,'R');
          e->insert_after(city.ent(1,1));
        } else {
          announce("You must have 15 influence to recruit new troubleshooters.");
        }
        break;
      case XK_a:
        if (influence > 5) {
          influence -= 5;
          Dwarf* e = new Dwarf(1,1,'D');
          e->insert_after(city.ent(1,1));
        } else {
          announce("You must have 5 influence to recruit new infrareds.");
        }
        break;
      case XK_space:
        paused = !paused;
        break;
      case XK_q:
        if (gfx) gfx->destroy();
        break;
      default:
        break;
      }
    } else if (cur_page == &unitpage) {
      switch (ks) {
      case XK_Escape:
        cur_page = &mainpage;
        break;
      case XK_space:
        paused = !paused;
        break;
      default:
        break;
      }
    } else {
      assert(false);
    }
  }

  MapView mv;
  Hud hud;
  JobListing activelist;
  JobListing pendinglist;
  UnitListing unitlist;

  ComponentList mainpage;
  ComponentList unitpage;

  Component* cur_page;
};

int main(int argc, char** argv) {
  srand(time(NULL));
  if (argc < 2)
    cin >> city;
  else
    fstream(argv[1]) >> city;

  Graphics g;
  gfx = &g;
  ViewStack vs(&city);

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
