#include "entities/foods.hpp"

#include "components/foodstuff.hpp"
#include "components/item.hpp"
#include "components/renderable.hpp"
#include "components/position.hpp"

#include "entities/entity.hpp"

item::ItemProperties bread_properties = { "Bread", 1 };

ecs::Ent* make_bread(const Point& p) {
  auto r = new ecs::Ent;
  r->emplace<Position>(p);
  r->emplace<Foodstuff>(1200);
  r->emplace<item::Item>(bread_properties);
  r->emplace<Renderable>('%');
  return r;
}

item::ItemProperties lettuce_properties = { "Lettuce", 1 };

ecs::Ent* make_lettuce(const Point& p) {
  auto r = new ecs::Ent;
  r->emplace<Position>(p);
  r->emplace<Foodstuff>(200);
  r->emplace<item::Item>(lettuce_properties);
  r->emplace<Renderable>('%');
  return r;
}
