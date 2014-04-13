#include "hydroponics/hydroponics_table.hpp"
#include "entities/entity.hpp"
#include "components/item.hpp"
#include "components/renderable.hpp"
#include "components/furniture.hpp"

ItemProperties hydroponics_table_properties = {
    "Hydroponics Table",
    100
};

Ent* make_hydroponics_table(Point const& p) {
    Ent* e = new Ent;
    e->emplace<Renderable>('T');
    e->emplace<HydroponicsTable>(p);
    e->emplace<Item>(hydroponics_table_properties);
    return e;
}

