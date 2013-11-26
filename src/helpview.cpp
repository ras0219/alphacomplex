#include "helpview.hpp"
#include "viewstack.hpp"
#include "graphics.hpp"
#include "X11/keysym.h"

extern bool paused;

struct HelpText : Component {
  virtual void render(Graphics& g) {
    g.drawString(150, g.getHeight() - 5, "Press 'h' for help.", Graphics::DEFAULT);
  }
} helptext;

struct HelpInfo : Component {
  virtual void render(Graphics& g) {
    static vector<string> lines = {
      "Press space to pause.",
      "Press 'r' to hire a Troubleshooter.",
      "Press 'a' to hire an Infrared.",
      "Press 'u' to see the units list.",
      "Press Esc to return to the main screen.",
      "Press 'q' to quit.",
      "",
      "Good Luck!"
    };
    for (uint x=0;x<lines.size();++x) {
      g.drawString(5, 12+5+12*x, lines[x], Graphics::DEFAULT);
    }
  }
} helpinfo;

void HelpView::render(Graphics& g) {
  helptext.render(g);
  helpinfo.render(g);
}

void HelpView::handle_keypress(KeySym ks) {
  switch (ks) {
  case XK_Escape:
    vstk->pop();
    break;
  case XK_space:
    paused = !paused;
    break;
  default:
    break;
  }
}
