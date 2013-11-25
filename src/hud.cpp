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

#include "entity.hpp"

void UnitListing::render(Graphics& g) {
  int row = 1;
  char buf[60];
  for (auto e : AIEntity::ai_list) {
    e->description(buf, 60);
    g.drawString(5, 5+10*row, buf, Graphics::DEFAULT);
    ++row;
  }
}

void announce(const string& s) {
  msglog.push_back(s);
}
