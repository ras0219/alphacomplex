#include "foodstuff.hpp"

ItemProperties breadprops = { "Bread", 1 };

Ent* make_bread(const Position& p) {
  Ent* r = new Ent;
  r->add(new PositionComp(p));
  r->add(new Foodstuff(1200));
  r->add(new Item(breadprops));
  return r;
}
