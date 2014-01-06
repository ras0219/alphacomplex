#pragma once

#include "ai.hpp"

template<class T>
struct DeleteAI : AIScript {
  DeleteAI(T* t) : ptr(t) { }

  virtual int start(AI* ai) {
    delete ptr;
    return complete(ai);
  }

  T* ptr;
};

template<class T>
inline std::shared_ptr<DeleteAI<T>> make_deleteai(T* t) { return make_shared<DeleteAI<T>>(t); }
