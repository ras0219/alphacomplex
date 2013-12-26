#pragma once

#include <type_traits>
#include <tuple>

namespace rasutil {
  template<size_t N>
  struct _apply {
    template<class F, class T, class...Args2>
    static inline auto apply(F&& f, T&& tup, Args2&&...args2)
      -> decltype(_apply<N - 1>::apply(std::forward<F>(f), std::forward<T>(tup), std::get<N - 1>(tup), std::forward<Args2>(args2)...))
    {
      return _apply<N - 1>::apply(std::forward<F>(f), std::forward<T>(tup), std::get<N - 1>(tup), std::forward<Args2>(args2)...);
    }
  };

  template<>
  struct _apply<0> {
    template<class F, class T, class...Args2>
    static inline typename std::result_of<F&&(Args2&&...)>::type
      apply(F&& f, T&&, Args2&&...args2)
    {
      return f(std::forward<Args2>(args2)...);
    }
  };

  template<class F, class T>
  inline auto apply_tuple(F&& f, T&& tup)
    -> decltype (_apply<std::tuple_size<T>::value>::apply(std::forward<F>(f), std::forward<T>(tup)))
  {
    return _apply<std::tuple_size<T>::value>::apply(std::forward<F>(f), std::forward<T>(tup));
  }
}
