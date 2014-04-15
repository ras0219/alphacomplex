#pragma once

#include "components/component.hpp"

namespace storage {

    /// Describes the properties of a storage component.
    struct StorageInfo {
        ///@todo allow filtering by item type
    };

    struct Storage : ecs::ComponentCRTP<ecs::Component::Storage, Storage> {
        StorageInfo info;
    };

}
