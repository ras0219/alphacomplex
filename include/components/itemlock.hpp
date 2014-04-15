#pragma once

#include <cassert>
#include <vector>
#include <memory>

namespace item {

    struct Item;

    struct unlock_item {
        void operator()(Item* i);
    };

    /// RAII Wrapper to cleanly manipulate item locks.
    struct ItemLock {
        /// Construct with no item
        ItemLock() {}
        /// Construct with no item
        ItemLock(std::nullptr_t) {}
        /// Construct with item `i`. Item `i` should be locked before this is called.
        explicit ItemLock(Item* i) : ptr(i) {}
        /// Transfer lock ownership.
        ItemLock(ItemLock&& i) : ptr(std::move(i.ptr)) {}
        /// Locks cannot be assigned.
        ItemLock& operator=(const ItemLock&) = delete;
        /// Transfer lock ownership and release current lock (if any).
        inline ItemLock& operator=(ItemLock&& i) {
            std::swap(ptr, i.ptr);
            return *this;
        }

        /// Convenience method
        Item& operator*() { return *ptr; }
        /// Convenience method
        Item* operator->() { return ptr.get(); }

        /// Allow the syntax `if (itemlock) { ... }`
        inline explicit operator bool() { return ptr.get() != nullptr; }

        /// Retrieve the underlying Item* and release responsibility for unlocking
        Item* release() { return ptr.release(); }
        /// Retrieve the underlying Item*
        Item* get() { return ptr.get(); }
        /// Unlock the underlying Item* and release responsibility.
        void reset() { return ptr.reset(); }

        /// Remove the item from its container and return the parent lock
        ItemLock remove_from();
        /// Consume another item lock and insert this item into the other.
        void insert_into(ItemLock&& o);
        /// Delete the underlying item object (delayed). Return the parent lock.
        ItemLock delete_reset();

        /// Commit all delayed deletions.
        static void finalize_deletes();

    private:
        std::unique_ptr<Item, unlock_item> ptr;

        static std::vector<ItemLock> to_delete;
    };

}
