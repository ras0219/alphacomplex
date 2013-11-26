#include "hud.hpp"
#include "graphics.hpp"

#include <sstream>
#include <deque>

using namespace std;

deque<string> msglog{"City begin.", "Placeholder.", "All systems nominal."};

int influence = 0;

void Hud::render(Graphics& g) {
  stringstream out;
  out << "Influence: " << influence;
  g.drawString(5, g.getHeight()-5, out.str(), Graphics::DEFAULT);

  auto sz = msglog.size();

  for (int x=1;x<4;++x)
    g.drawString(5, g.getHeight()-5-10*x, msglog[sz-x], Graphics::DEFAULT);

}

void announce(const string& s) {
  msglog.push_back(s);
}

Hud hud;
