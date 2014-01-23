#pragma once

namespace rasutil {

  /// Thunks a member function pointer into a normal functor
  /// Specifically, converts it into an empty functor with an operator() that
  /// takes a T* as its first argument.
  template<class F>
  struct thunk_func;

  /// Implementation specialization of thunk_func<F>
  template<class R, class C, class...Args>
  struct thunk_func<R(C::*)(Args...)> {
    using func_type = R(C::*)(Args...);

    thunk_func(func_type f) : f(f) {}

    inline R operator()(C* c, Args...args) {
      return (c->*f)(std::forward<Args>(args)...);
    }

  private:
    func_type f;
  };

  template<class R, class C, class...Args>
  thunk_func<R(C::*)(Args...)> thunk(R(C::*f)(Args...)) {
    return thunk_func<R(C::*)(Args...)>(f);
  }
}