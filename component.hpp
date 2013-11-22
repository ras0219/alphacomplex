#pragma once

struct Graphics;

struct Component {
  virtual void render(Graphics&) = 0;
};

