#include "views/mainview.hpp"
#include "views/viewstack.hpp"
#include "views/mapview.hpp"
#include "views/hud.hpp"
#include "views/helpview.hpp"
#include "views/unitview.hpp"
#include "views/announceview.hpp"
#include "views/designview.hpp"
#include "views/statustext.hpp"

#include "joblist.hpp"
#include "entities/citizen.hpp"

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

JobListing pendinglist(20, &jobs, "Pending Jobs");

struct HelpText : Widget {
  virtual void render(Graphics& g);
};

extern HelpText helptext;

void MainView::render(Graphics& g) {
  mv->render(g);
  hud.render(g);
  pendinglist.render(g);
  helptext.render(g);
  statustext.render(g);
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
      // Citizen* e = new Citizen(1,1,Security::RED, city);
      // e->insert_after(city.ent(1,1));
      city.ent(1,1).insert(new_citizen({1,1,&city}, Security::RED));
    } else {
      announce("You must have 15 influence to recruit new troubleshooters.");
    }
    break;
  case KEY_e:
    if (influence >= 5) {
      influence -= 5;
      // Citizen* e = new Citizen(1,1,Security::INFRARED, city);
      // e->insert_after(city.ent(1,1));
      city.ent(1,1).insert(new_citizen({1,1,&city}, Security::INFRARED));
    } else {
      announce("You must have 5 influence to recruit new infrareds.");
    }
    break;
  case KEY_Tab:
    mv->next_mode();
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
