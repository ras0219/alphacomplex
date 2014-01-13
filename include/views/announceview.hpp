#pragma once

#include "baseview.hpp"

struct AnnounceView : BaseView {
  AnnounceView(struct ViewStack* vs);

  virtual void render_body(Graphics& g, render_box const& pos) override;
};
