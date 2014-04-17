#include "components/item.hpp"
#include "components/itemlock.hpp"
#include "components/position.hpp"
#include "components/furniture.hpp"

namespace item {

    void unlock_item::operator()(Item* i) {
        assert(i->locked);
        i->locked = false;
        if (i->container)
            (*this)(i->container);
    }

    ItemLock ItemLock::remove_from() {
        auto parent = ptr->container;
        ptr->remove_from();
        return ItemLock(parent);
    }

    void ItemLock::insert_into(ItemLock&& o) {
        assert(o);
        ptr->insert_into(o.release());
    }

    ItemLock ItemLock::delete_reset() {
        if (ptr->container) {
            ItemLock r = remove_from();
            to_delete.emplace_back(std::move(*this));
            return r;
        } else {
            to_delete.emplace_back(std::move(*this));
            return nullptr;
        }
    }

    void ItemLock::finalize_deletes() {
        for (auto& l : to_delete) {
            auto p = l.get();
            l.reset();
            delete p->parent;
        }
        to_delete.clear();
    }

    Point Item::pos() const {
        if (container != nullptr) {
            return container->pos();
        } else {
            if (parent->has<struct Position>()) {
                return parent->assert_get<struct Position>()->as_point();
            }
            if (parent->has<struct Furniture>()) {
                return parent->assert_get<struct Furniture>()->as_point();
            }
            assert(false);
            std::terminate();
        }
    }

    void Item::on_remove() {
        if (container) remove_from();

        // For now, just terminate the program if we try to delete a filled container
        if (!items.empty()) {
            assert(false);
            std::terminate();
        }
    }

    ItemLock Item::try_lock() {
        if (locked)
            return ItemLock(nullptr);
        if (container == nullptr) {
            assert(!locked);
            locked = true;
            return ItemLock(this);
        }
        auto l = container->try_lock();
        if (l) {
            l.release();
            assert(!locked);
            locked = true;
            return ItemLock(this);
        }
        return ItemLock(nullptr);
    }

    std::vector<ItemLock> ItemLock::to_delete;

}
