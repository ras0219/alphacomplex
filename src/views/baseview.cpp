#include "views/baseview.hpp"
#include "views/defaultlayout.hpp"

BaseView::BaseView(struct ViewStack* vs) : vstk(vs) {}

void BaseView::render(Graphics& g, render_box const& pos) {
  render_box pos2 = DefaultLayout::render_layout(this, g, pos);
  render_body(g, pos2);
}

void BaseView::handle_keypress(KeyboardKey ks) {
  nav.handle_keypress(ks);
}
