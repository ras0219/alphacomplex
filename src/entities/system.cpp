#include "entities/system.hpp"

namespace ecs {

    std::vector<System*> createSystems(City* city) {
        std::vector<System*> ret;

        auto& factories = SystemFactory::getSystemFactories();
        for (auto it : factories) {
            ret.push_back((*it)(city));
        }

        return ret;
    }

    std::vector<SystemFactory*>& SystemFactory::getSystemFactories() {
        static std::vector<SystemFactory*> factories;
        return factories;
    }

    SystemFactory::SystemFactory() {
        getSystemFactories().push_back(this);
    }

}
