#pragma once

#include "overlay.hpp"
#include "entities/system.hpp"
#include "storage.hpp"
#include <unordered_set>
#include <vector>
#include <memory>

struct City;

namespace item { struct Item; }
namespace job { struct Job; }

namespace storage {
    struct Storage;

    struct StorageSystem : ecs::System {
        StorageSystem(City* c);

        virtual void update() override;
        virtual void insert(ecs::Ent* e) override;
        virtual void erase(ecs::Ent* e) override;

        static ecs::CRTPSystemFactory<StorageSystem, true> factory;

    private:
        void delayed_update();
        void complete_jobs();
        void build_info_map();
        void issue_jobs();

        City* city;
        Overlay<void*> info_map;
        std::unordered_set<ecs::Ent*> ents;
        using jobs_t = std::vector< std::pair<item::Item*, std::shared_ptr<job::Job>> >;
        jobs_t jobs;

        size_t timer;
    };

}
