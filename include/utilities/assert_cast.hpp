#pragma once

#include <algorithm>
#include <vector>

/// static_cast in Release, check via dynamic_cast in Debug.
template<class T, class U>
T* assert_cast(U* u) {
#ifndef NDEBUG
    T* t = dynamic_cast<T*>(u);
    if (t == nullptr) {
        std::abort();
    }
    return t;
#else
    return static_cast<T*>(u);
#endif
}

/// Overload to reinterpret a vector. Performs a check in debug mode, no check in release.
///@warning The stars must be aligned for this to work!
///@warning No multiple inheritance
///@warning The pointers must not require adjustment
///@warning std::vector must not be specialized for U* or T*
template<class T, class U, class = std::enable_if_t<std::is_base_of<U,T>::value>>
std::vector<T*>& assert_cast(std::vector<U*>& us) {
#ifndef NDEBUG
    if (std::any_of(us.begin(), us.end(), [](U* u) { return dynamic_cast<T*>(u) == nullptr; })) {
        std::abort();
    }
#endif
    return reinterpret_cast<std::vector<T*>&>(us);
}