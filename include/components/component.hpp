#pragma once

#include <cassert>
#include <vector>
#include <algorithm>

namespace ecs {

    struct Component {
        enum Kind {
            Skilled,
            Renderable,
            AI,
            Needs,
            Clearance,
            Descriptable,
            Position,
            CitizenName,
            Movable,
            Inventory,
            Room,
            Furniture,
            Foodstuff,
            Pharmaceutical,
            Item,
            JobProvider,
            Storage
        };

        Component(Kind k) : kind(k), parent(nullptr) { }
        virtual ~Component() {}

        virtual void on_add() {}
        virtual void on_remove() {}

        Component& operator=(const Component&) = delete;

        template<class T>
        T& as() {
            assert(kind == T::StaticKind);
            return static_cast<T&>(*this);
        }
        template<class T>
        const T& as() const {
            assert(kind == T::StaticKind);
            return static_cast<const T&>(*this);
        }

        const Kind kind;
        struct Ent* parent;
    };

    template<Component::Kind K, class T>
    struct ComponentCRTP : Component {
        static const Component::Kind StaticKind = K;
        ComponentCRTP() : Component(K) {
            instances.push_back(&this->as<T>());
        }
        ~ComponentCRTP() { instances.erase(std::find(instances.begin(), instances.end(), &this->as<T>())); }

        using set_t = std::vector<T*>;
        using iterator = typename set_t::iterator;
        static set_t instances;
    };
    template<Component::Kind K, class T>
    typename ComponentCRTP<K, T>::set_t ComponentCRTP<K, T>::instances;

}
