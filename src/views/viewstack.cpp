#include "views/viewstack.hpp"
#include <cassert>

void ViewStack::render(Graphics& g, render_box const& pos) {
  vstack.back()->render(g, pos);
}

void ViewStack::handle_keypress(KeyboardKey ks) {
  vstack.back()->handle_keypress(ks);
}

void ViewStack::push(View* v) {
  vstack.push_back(v);
}

void ViewStack::pop() {
  assert(vstack.size() > 1);
  vstack.pop_back();
}

void ViewStack::reset() {
  assert(vstack.size() > 0);
  vstack.erase(vstack.begin()+1, vstack.end());
}
