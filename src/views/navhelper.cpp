#include "views/navhelper.hpp"
#include "graphics.hpp"

void NavHelper::render(Graphics& g, render_box const& pos) {
  int y = pos.y + pos.h - 1;

  for (auto&& p : nav_map) {
    g.drawString(pos.x, y, std::get<0>(p.second));
    y--;
  }
}

bool NavHelper::handle_keypress(KeyboardKey ks) {
  auto it = nav_map.find(ks);
  if (it != nav_map.end()) {
    std::get<1>(it->second)();
    return true;
  } else {
    return false;
  }
}

void NavHelper::register_key(KeyboardKey ks, const std::string& desc, cb_t cb) {
    assert(nav_map.find(ks) == nav_map.end());
    nav_map.emplace(ks, std::make_tuple(desc, cb));
}

