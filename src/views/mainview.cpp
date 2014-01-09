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

MainView::MainView(ViewStack* vs, City* c) : vstk(vs), city(c), mv(c->getXSize(), c->getYSize(), c) {
  hview = new HelpView(vs);
  uview = new UnitView(vs);
  aview = new AnnounceView(vs);
  dview = new DesignView(vs, &mv, c);

  nav.register_key(KEY_h, "[h] Help", [this]() { vstk->push(hview); });
  nav.register_key(KEY_u, "[u] Unit View", [this]() { vstk->push(uview); });
  nav.register_key(KEY_d, "[d] Designations", [this]() { vstk->push(dview); });
  nav.register_key(KEY_a, "[a] Announcements", [this]() { vstk->push(aview); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
  nav.register_key(KEY_Tab, "[Tab] Map Mode", [this]() { mv.next_mode(); });
  nav.register_key(KEY_q, "[q] Quit", [this]() { if (gfx) gfx->destroy(); });
}

JobListing pendinglist(20, &jobs, "Pending Jobs");

void MainView::render(Graphics& g) {
  mv.render(g);
  hud.render(g);
  pendinglist.render(g);
  HelpText::instance.render(g);
  statustext.render(g);
  nav.render(g);
}

void MainView::handle_keypress(KeySym ks) {
  if (nav.handle_keypress(ks)) return;

  switch (ks) {
  case KEY_r:
    if (influence >= 15) {
      influence -= 15;
      city->ent(1, 1).insert(new_citizen({ 1, 1, city }, Security::RED));
    } else {
      announce("You must have 15 influence to recruit new troubleshooters.");
    }
    break;
  case KEY_e:
    if (influence >= 5) {
      influence -= 5;
      city->ent(1, 1).insert(new_citizen({ 1, 1, city }, Security::INFRARED));
    } else {
      announce("You must have 5 influence to recruit new infrareds.");
    }
    break;
  default:
    cerr << ks << endl;
    break;
  }
}
