#include "views/hud.hpp"
#include "graphics.hpp"
#include "clock.hpp"

#include <sstream>
#include <deque>

using namespace std;

int influence = 0;

void Hud::render(Graphics& g, render_box const& pos) {
  stringstream out;
  out << "Influence: " << influence;
  
  g.drawString(pos.x, pos.y + pos.h - 1, out.str(), true, Graphics::colors_to_context[Graphics::GREEN]);
  g.drawString(pos.x + pos.w - 25, pos.y + pos.h - 1, format_time(gametime), false, Graphics::colors_to_context[Graphics::WHITE]);

  auto sz = A11s::instance.msgs.size();

  for (size_t x = 1; x < (size_t)pos.h && x <= sz; ++x)
	  g.drawString(pos.x, pos.y + pos.h - x - 1, A11s::instance.msgs[sz - x], true, Graphics::colors_to_context[Graphics::BLUE]);

}

void A11s::announce(const string& s) {
  msgs.push_back(s);
}

void announce(const string& s) { A11s::instance.announce(s); }

A11s A11s::instance;
Hud Hud::instance;
