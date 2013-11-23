#pragma once

struct Component {
  virtual void render(struct Graphics&) = 0;
};

