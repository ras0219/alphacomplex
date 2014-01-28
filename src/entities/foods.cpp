#include "entities/foods.hpp"

#include "components/foodstuff.hpp"
#include "components/item.hpp"
#include "components/renderable.hpp"
#include "components/position.hpp"

#include "entities/entity.hpp"

ItemProperties bread_properties = { "Bread", 1 };

Ent* make_bread(const Point& p) {
  Ent* r = new Ent;
  r->add(new Position(p));
  r->add(new Foodstuff(1200));
  r->add(new Item(bread_properties));
  r->add(new Renderable('%'));
  return r;
}

ItemProperties lettuce_properties = { "Lettuce", 1 };

Ent* make_lettuce(const Point& p) {
  Ent* r = new Ent;
  r->add(new Position(p));
  r->add(new Foodstuff(200));
  r->add(new Item(lettuce_properties));
  r->add(new Renderable('%'));
  return r;
}
