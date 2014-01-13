#pragma once

#include "graphics.hpp"
#include "widget.hpp"
#include "statustext.hpp"
#include "hud.hpp"

/// Provides default layout functions to conforming views.
/// Specifically, the View V must allow `declval<V*>()->nav.render(g, pos)`.
struct DefaultLayout {

  /// Render using the default layout
  ///
  /// @param self The conforming view (to access `self->nav`)
  /// @param g The graphics context
  /// @param pos The geometry to place the layout into
  /// @return The remaining geometry for the view-specific data
  template<class /* View */ V>
  static inline render_box render_layout(V* self, Graphics& g, render_box const& pos) {
    render_box pos2;

    pos2 = pos;
    pos2.set_width(20, render_box::right);
    pos2.set_height(1, render_box::top);
    StatusText::instance.render(g, pos2);

    pos2 = pos;
    pos2.set_width(30, render_box::right);
    pos2.set_height(10, render_box::bottom);
    self->nav.render(g, pos2);

    pos2 = pos;
    pos2.set_height(10, render_box::bottom);
    Hud::instance.render(g, pos2);

    pos2 = pos;
    pos2.shrink_bottom(10);
    pos2.shrink_top(1);
    return pos2;
  }
};