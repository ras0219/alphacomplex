#pragma once

#include <vector>
#include <cassert>

struct City;

namespace ecs {

    struct Ent;

    struct System {
        virtual void update() = 0;
        virtual void insert(Ent* e) = 0;
        virtual void erase(Ent* e) = 0;
    };

    std::vector<System*> createSystems(City* city);

    struct SystemFactory {
        SystemFactory();
        virtual System* operator()(City* c) = 0;

        static std::vector<SystemFactory*>& getSystemFactories();
    };

    template<class Sys, bool needsCity = false>
    struct CRTPSystemFactory : SystemFactory {
        CRTPSystemFactory() : g_singleton(nullptr) {}

        virtual System* operator()(City*) override {
            assert(g_singleton == nullptr);
            g_singleton = new Sys();
            return g_singleton;
        }

        Sys* singleton() {
            assert(g_singleton != nullptr);
            return g_singleton;
        }

    private:
        Sys* g_singleton;
    };

    template<class Sys>
    struct CRTPSystemFactory<Sys, true> : SystemFactory {
        CRTPSystemFactory() : g_singleton(nullptr) {}

        virtual System* operator()(City* city) override {
            assert(g_singleton == nullptr);
            g_singleton = new Sys(city);
            return g_singleton;
        }

        Sys* singleton() {
            assert(g_singleton != nullptr);
            return g_singleton;
        }

    private:
        Sys* g_singleton;
    };

}