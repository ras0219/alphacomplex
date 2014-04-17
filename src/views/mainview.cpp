#include "views/mainview.hpp"

#include "city.hpp"
#include "views/viewstack.hpp"
#include "views/mapview.hpp"
#include "views/helpview.hpp"
#include "views/unitview.hpp"
#include "views/announceview.hpp"
#include "views/designview.hpp"

#include "views/defaultlayout.hpp"

#include "job/joblist.hpp"
#include "entities/citizen.hpp"
#include "entities/foods.hpp"

extern Graphics* gfx;
extern bool paused;

MainView::MainView(ViewStack* vs, City* c) : BaseView(vs), city(c), mv(c->getXSize(), c->getYSize(), c) {
  hview = new HelpView(vs);
  uview = new UnitView(vs);
  aview = new AnnounceView(vs);
  dview = new DesignView(vs, &mv, c);

  nav.register_key(KEY_h, "[h] Help", [this]() { vstk->push(hview); });
  nav.register_key(KEY_u, "[u] Unit View", [this]() { vstk->push(uview); });
  nav.register_key(KEY_d, "[d] Designations", [this]() { vstk->push(dview); });
  nav.register_key(KEY_a, "[a] Announcements", [this]() { vstk->push(aview); });
  nav.register_key(KEY_f, "[f] Purchase Food", [this]() {
      if (influence >= 1) {
          influence -= 1;
          city->ent(1, 1).insert(make_bread({ city, 1, 1 }));
      } else {
          announce("You must have 1 influence to purchase bread.");
      }
  });

  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
  nav.register_key(KEY_Tab, "[Tab] Map Mode", [this]() {
      mv.next_mode();
      announce("Changed Map Mode.");
  });
  nav.register_key(KEY_q, "[q] Quit", [this]() { if (gfx) gfx->destroy(); });
  nav.register_key(KEY_r, "[r] Recruit R", [this]() {
      if (influence >= 10) {
          influence -= 10;
          new_citizen({ city, 1, 1 }, Security::RED);
      } else {
          announce("You must have 10 influence to recruit new troubleshooters.");
      }
  });
  nav.register_key(KEY_e, "[e] Recruit IR", [this]() {
      if (influence >= 5) {
          influence -= 5;
          new_citizen({ city, 1, 1 }, Security::INFRARED);
      } else {
          announce("You must have 5 influence to recruit new infrareds.");
      }
  });
}

job::JobListing pendinglist(&job::JobList::getJoblist(), "Pending Jobs");

void MainView::render_body(Graphics& g, render_box const& pos) {
  render_box pos2 = pos;

  pos2.shrink_right(30);
  mv.render(g, pos2);

  pos2.set_width(30, render_box::right);
  pos2.x += 30;
  pendinglist.render(g, pos2);
}

void MainView::handle_keypress(KeyboardKey ks) {
  if (nav.handle_keypress(ks)) return;

  cerr << ks << endl;
}
