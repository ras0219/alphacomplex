#include "hud.hpp"
#include "graphics.hpp"

#include <sstream>

using namespace std;

int influence = 0;

void Hud::render(Graphics& g) {
  stringstream out;
  out << "Influence: " << influence;
  g.drawString(5, g.getHeight()-5, out.str(), Graphics::DEFAULT);
}
