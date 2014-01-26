#pragma once

#include <memory>

template<class T, void(*Deleter)(T*)>
struct Resource : std::unique_ptr<T> {
  Resource() {}
  Resource(T* t) : std::unique_ptr<T>(t, Deleter) {}
};
