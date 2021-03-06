#pragma once

#include "components/component.hpp"

#include <unordered_set>
#include <unordered_map>

namespace ecs {

    struct System;

    struct Ent {
        typedef std::unordered_map<int, Component*> map_t;
        typedef std::unordered_set<System*> set_t;

        ~Ent();

        /// Overload for get<T>()
        ///@deprecated
        ///@see get
        template<class T>
        T* assert_get() {
            return get<T>();
        }

        template<class T>
        T* get() {
            assert(has<T>());
            return static_cast<T*>(compmap[T::StaticKind]);
        }
        template<class T>
        T* has_get() {
            auto it = compmap.find(T::StaticKind);
            if (it == compmap.end())
                return nullptr;
            return static_cast<T*>(it->second);
        }

        template<class T>
        T* remove() {
            T* ret = get<T>();
            remove(ret);
            return ret;
        }

        template<class T>
        bool has() {
            return compmap.find(T::StaticKind) != compmap.end();
        }

        template<class...Ts>
        std::tuple<Ts*...> get_tuple() {
            return std::make_tuple(std::get<Ts>()...);
        }

        bool has(Component::Kind k);
        void add(Component* comp);
        void add(System* sys);
        void remove(Component* comp);

        template<class C, class...Args>
        void emplace(Args&&...args) {
            add(new C(std::forward<Args>(args)...));
        }

        template<class Sys>
        void emplace_system() {
            add(Sys::factory.singleton());
        }

        map_t compmap;
        set_t sysset;
    };
}
