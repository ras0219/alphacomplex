#pragma once

struct box {
  int x, y, w, h;
};

struct Widget {
  virtual void render(struct Graphics&) = 0;
};
