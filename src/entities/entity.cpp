#include "entities/system.hpp"
#include "entities/entity.hpp"

#include <algorithm>
#include <list>
#include <map>
#include <tuple>
#include <cassert>

namespace ecs {

    Ent::~Ent() {
        for (auto p : compmap)
            p.second->on_remove();
        for (auto p : compmap)
            delete p.second;
        for (auto sys : sysset)
            sys->erase(this);
    }

    bool Ent::has(Component::Kind k) {
        return compmap.find(k) != compmap.end();
    }

    void Ent::add(Component* comp) {
        comp->parent = this;
        compmap[comp->kind] = comp;
        comp->on_add();
    }
    void Ent::add(System* sys) {
        sysset.insert(sys);
        sys->insert(this);
    }

    void Ent::remove(Component* comp) {
        assert(comp);
        assert(compmap[comp->kind] == comp);
        comp->on_remove();
        compmap[comp->kind] = nullptr;
        comp->parent = nullptr;
    }

}
