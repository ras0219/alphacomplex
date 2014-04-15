#pragma once

#include "components/component.hpp"
#include "point.hpp"

struct Furniture : ecs::ComponentCRTP<ecs::Component::Furniture, Furniture> {
  Furniture(const Point& pos) : pos(pos) {}

  virtual void on_add() override;
  virtual void on_remove() override;

  inline int x() const { return pos.x; }
  inline int y() const { return pos.y; }
  inline Point as_point() const { return pos; }

  Point pos;
};
