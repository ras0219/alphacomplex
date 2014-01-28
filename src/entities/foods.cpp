#include "entities/foods.hpp"

#include "components/foodstuff.hpp"
#include "components/item.hpp"
#include "components/renderable.hpp"
#include "components/position.hpp"

#include "entities/entity.hpp"

ItemProperties breadprops = { "Bread", 1 };

Ent* make_bread(const Point& p) {
  Ent* r = new Ent;
  r->add(new Position(p));
  r->add(new Foodstuff(1200));
  r->add(new Item(breadprops));
  r->add(new Renderable('%'));
  return r;
}
