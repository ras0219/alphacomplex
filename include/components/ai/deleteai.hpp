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
inline DeleteAI<T>* new_deleteai(T* t) { return new DeleteAI<T>(t); }
