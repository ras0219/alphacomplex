#pragma once

#include "view.hpp"
#include <map>
#include <tuple>
#include <functional>

struct NavHelper : Widget {
  using cb_t = std::function<void()>;
  using nav_entry_t = std::tuple<std::string, cb_t>;
  using nav_map_t = std::map<KeySym, nav_entry_t>;

  void register_key(KeySym, const std::string&, cb_t cb);

  virtual void render(Graphics& g, render_box const& pos) override;
  bool handle_keypress(KeySym ks);

  nav_map_t nav_map;
};