#include "mainview.hpp"
#include "viewstack.hpp"
#include "mapview.hpp"
#include "joblist.hpp"
#include "hud.hpp"
#include "helpview.hpp"
#include "unitview.hpp"
#include "announceview.hpp"
#include "designview.hpp"
#include "citizen.hpp"

extern Graphics* gfx;
extern bool paused;

MapView *mv = nullptr;

MainView::MainView(ViewStack* vs, City& c) : vstk(vs), city(c) {
  if (mv == nullptr)
    mv = new MapView(city.getXSize(), city.getYSize(), &city);
  hview = new HelpView(vs);
  uview = new UnitView(vs);
  aview = new AnnounceView(vs);
  dview = new DesignView(vs, mv, c);
}

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
  case KEY_h:
    vstk->push(hview);
    break;
  case KEY_u:
    vstk->push(uview);
    break;
  case KEY_d:
    vstk->push(dview);
    break;
  case KEY_a:
    vstk->push(aview);
    break;
  case KEY_r:
    if (influence >= 15) {
      influence -= 15;
      Citizen* e = new Citizen(1,1,Security::RED, city);
      e->insert_after(city.ent(1,1));
    } else {
      announce("You must have 15 influence to recruit new troubleshooters.");
    }
    break;
  case KEY_e:
    if (influence >= 5) {
      influence -= 5;
      Citizen* e = new Citizen(1,1,Security::INFRARED, city);
      e->insert_after(city.ent(1,1));
    } else {
      announce("You must have 5 influence to recruit new infrareds.");
    }
    break;
  case KEY_space:
    paused = !paused;
    break;
  case KEY_q:
    if (gfx)
      gfx->destroy();
    break;
  default:
    cerr << ks << endl;
    break;
  }
}
