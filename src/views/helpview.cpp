#include "views/helpview.hpp"
#include "views/viewstack.hpp"
#include "graphics.hpp"

extern bool paused;

struct HelpText : Widget {
  virtual void render(Graphics& g) {
    g.drawString(150, g.getHeight() - 5, "Press 'h' for help.", Graphics::DEFAULT);
  }
} helptext;

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
      g.drawString(5, 12+5+12*x, help_prgh[x], Graphics::DEFAULT);
    }
  }
} helpinfo;

void HelpView::render(Graphics& g) {
  helptext.render(g);
  helpinfo.render(g);
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
