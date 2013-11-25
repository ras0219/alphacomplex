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
  for (auto e : AIEntity::ai_list) {
    g.drawString(5, 5+10*row, e->rawname(), Graphics::DEFAULT);
    ++row;
  }

  stringstream ss;
  for (char c = 160; c != 0; ++c)
    ss << c;
  stringstream ss3;
  for (char c = 10; c < '0'; ++c)
    ss3 << c;
  stringstream ss2;
  for (char c = 160; c != 0; ++c)
    ss2 << ((unsigned char)c % 10);

  g.drawString(5, 60, ss2.str(), Graphics::DEFAULT);
  g.drawString(5, 70, ss.str(), Graphics::DEFAULT);
  g.drawString(5, 80, ss3.str(), Graphics::DEFAULT);

  char buf[][4] = {{ CORNER_NW, HBAR, CORNER_NE, 0 },
                  { VBAR, '?', VBAR, 0 },
                  { CORNER_SW, HBAR, CORNER_SE, 0 }};

  g.drawString(5, 90, buf[0], Graphics::DEFAULT);
  g.drawString(5, 102, buf[1], Graphics::DEFAULT);
  g.drawString(5, 114, buf[2], Graphics::DEFAULT);
  for (int x=0;x<3;++x)
    g.drawChar(5 + x*6, 126, buf[2][x], Graphics::DEFAULT);
  for (int x=0;x<3;++x)
    g.drawChar(5 + x*7, 138, buf[2][x], Graphics::DEFAULT);
  for (int x=0;x<3;++x)
    g.drawChar(5 + x*8, 150, buf[2][x], Graphics::DEFAULT);
}

void announce(const string& s) {
  msglog.push_back(s);
}
