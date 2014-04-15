#pragma once

#include "point.hpp"
#include "components/furniture.hpp"

namespace item {
    struct ItemProperties;
}
namespace ecs {
    struct Ent;
}
namespace hydroponics {
    extern item::ItemProperties hydroponics_table_properties;

    ecs::Ent* make_hydroponics_table(const Point& p);

    struct HydroponicsTable : Furniture {
        enum GrowthStage {
            not_planted,
            plant_requested,
            planted,
            stage1,
            stage2,
            stage3,
            ready
        };

        HydroponicsTable(Point p) : Furniture(p), stage(not_planted) {}

        GrowthStage stage;
    };

}