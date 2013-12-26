#pragma once

#include <type_traits>
#include "apply_tuple.hpp"

namespace rasutil {

  template<class F, class...Args>
  struct curried_func {
    curried_func(F&& f, Args&&...args)
    : f(std::forward<F>(f)), tup(make_tuple(std::forward<Args>(args)...)) {}

    template<class...Args2>
    std::result_of<F(Args&&..., Args2&&...)> operator()(Args2&&...args2) {
      return apply_tuple(f, tup, std::forward<Args2>(args2)...);
    }

  private:
    F&& f;
    std::tuple<Args&&...> tup;
  };

  template<class F, class...Args>
  curried_func<F, Args...> curry_func(F&& f, Args&&...args) {
    return curried_func<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
  }

}