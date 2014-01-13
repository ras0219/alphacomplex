#pragma once

struct render_box {
  int x, y, w, h;
};

struct Widget {
  virtual void render(struct Graphics&) = 0;
};
