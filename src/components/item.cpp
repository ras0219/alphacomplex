#include "components/item.hpp"
#include "components/position.hpp"
#include "components/furniture.hpp"

Point Item::pos() const {
  if (container != nullptr) {
    return container->pos();
  } else {
    if (parent->has<struct Position>()) {
      return parent->assert_get<struct Position>()->as_point();
    }
    if (parent->has<struct Furniture>()) {
      return parent->assert_get<struct Furniture>()->as_point();
    }
    assert(false);
    std::terminate();
  }
}