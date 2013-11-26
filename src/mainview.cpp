#include "mainview.hpp"
#include "viewstack.hpp"
#include "mapview.hpp"
#include "city.hpp"
#include "joblist.hpp"
#include "hud.hpp"
#include "helpview.hpp"
#include "unitview.hpp"
#include "citizen.hpp"

#include <X11/keysym.h>

extern Graphics* gfx;
extern bool paused;

MapView *mv = nullptr;

MainView::MainView(ViewStack* vs) : vstk(vs) {
  if (mv == nullptr)
    mv = new MapView(city.getXSize(), city.getYSize(), &city);
  hview = new HelpView(vs);
  uview = new UnitView(vs);
}

Hud hud;

JobListing activelist(20, &active_jobs, "Active Jobs");
JobListing pendinglist(200, &jobs, "Pending Jobs");

struct HelpText : Component {
  virtual void render(Graphics& g);
};

extern HelpText helptext;

void MainView::render(Graphics& g) {
  mv->render(g);
  hud.render(g);
  activelist.render(g);
  pendinglist.render(g);
  helptext.render(g);
}

void MainView::handle_keypress(KeySym ks) {
  switch (ks) {
  case XK_h:
    vstk->push(hview);
    break;
  case XK_u:
    vstk->push(uview);
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
    if (gfx)
      gfx->destroy();
    break;
  default:
    cerr << ks << endl;
    break;
  }
}
