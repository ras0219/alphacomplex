#include "views/navhelper.hpp"
#include "graphics.hpp"

void NavHelper::render(Graphics& g, render_box const& pos) {
  //int x = g.getWidth() - 25;
  //int y = g.getHeight() - nav_map.size() - 4;
  int y = pos.y;
  for (auto p : nav_map) {
    g.drawString(pos.x, y, std::get<0>(p.second));
    y++;
  }
}

bool NavHelper::handle_keypress(KeySym ks) {
  auto it = nav_map.find(ks);
  if (it != nav_map.end()) {
    std::get<1>(it->second)();
    return true;
  } else {
    return false;
  }
}

void NavHelper::register_key(KeySym ks, const std::string& desc, cb_t cb) {
  nav_map.emplace(ks, std::make_tuple(desc, cb));
}

