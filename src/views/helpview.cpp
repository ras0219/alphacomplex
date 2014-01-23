#include "views/helpview.hpp"
#include "views/viewstack.hpp"
#include "graphics.hpp"
#include "views/defaultlayout.hpp"

extern bool paused;

vector<string> help_prgh = {
  "== All Modes ==",
  "  Escape   - Return to the previous screen",
  "  Space    - Pause",
  "== Main Screen ==",
  "  d        - Digging mode",
  "  u        - Units Screen",
  "  h        - Help Screen",
  "  a        - Announcements Screen",
  "  r        - Recruit a new RED clearance citizen",
  "  e        - Recruit a new INFRARED clearance citizen",
  "  q        - Quit",
  "== Digging Screen ==",
  "  Arrows   - Move the cursor (the 'X')",
  "  Enter    - Toggle digging wall underneath cursor",
  "== Units Screen ==",
  "  Arrows   - Move the cursor (the '> <'s)",
  "  Enter    - Modify data cell",
  "  Tab      - Switch between Skills and Department assignment",
  "",
  "",
  "Good Luck!"
};

struct HelpInfo : StaticWidget<HelpInfo> {
  void render(Graphics& g, render_box const& pos) {
    for (uint x=0; x<help_prgh.size(); ++x) {
      g.drawString(pos.x, pos.y + x, help_prgh[x], Graphics::DEFAULT);
    }
  }
  static HelpInfo instance;
};
HelpInfo HelpInfo::instance;

HelpView::HelpView(ViewStack* vs) : vstk(vs) {
  nav.register_key(KEY_Escape, "[Esc] Back", [this]() { vstk->pop(); });
  nav.register_key(KEY_space, "[Spc] Pause", [this]() { paused = !paused; });
}

void HelpView::render(Graphics& g, render_box const& pos) {
  render_box pos2 = DefaultLayout::render_layout(this, g, pos);

  HelpInfo::instance.render(g, pos2);
}

void HelpView::handle_keypress(KeyboardKey ks) {
  nav.handle_keypress(ks);
}
