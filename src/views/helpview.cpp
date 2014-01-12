#include "views/helpview.hpp"
#include "views/viewstack.hpp"
#include "graphics.hpp"

extern bool paused;

void HelpText::render(Graphics& g) {
  g.drawString(22, g.getHeight() - 2, "Press 'h' for help.", Graphics::DEFAULT);
}
HelpText HelpText::instance;

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

struct HelpInfo : Widget {
  virtual void render(Graphics& g) {
    for (uint x=0; x<help_prgh.size(); ++x) {
      g.drawString(2, 1 + x, help_prgh[x], Graphics::DEFAULT);
    }
  }
  static HelpInfo instance;
};
HelpInfo HelpInfo::instance;

void HelpView::render(Graphics& g) {
  HelpText::instance.render(g);
  HelpInfo::instance.render(g);
}

void HelpView::handle_keypress(KeySym ks) {
  switch (ks) {
  case KEY_Escape:
    vstk->pop();
    break;
  case KEY_space:
    paused = !paused;
    break;
  default:
    break;
  }
}
