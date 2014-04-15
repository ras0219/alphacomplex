#include "hydroponics/hydroponics_table.hpp"
#include "entities/entity.hpp"
#include "components/item.hpp"
#include "components/renderable.hpp"
#include "components/furniture.hpp"

namespace hydroponics {

    item::ItemProperties hydroponics_table_properties = {
        "Hydroponics Table",
        100
    };

    ecs::Ent* make_hydroponics_table(Point const& p) {
        ecs::Ent* e = new ecs::Ent;
        e->emplace<Renderable>('T');
        e->emplace<HydroponicsTable>(p);
        e->emplace<item::Item>(hydroponics_table_properties);
        return e;
    }

}
