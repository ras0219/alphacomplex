#pragma once

#include <memory>

template<class T, void(*Deleter)(T*)>
struct Resource {
  Resource() {}
  Resource(T* t) : ptr(t, Deleter) {}

  operator T*() { return ptr.get(); }
  operator T const*() const { return ptr.get(); }

  T& operator*() { return *ptr; }
  T const& operator*() const { return *ptr; }

  operator void const*() const { return ptr.get(); }

private:
  std::shared_ptr<T> ptr;
};
